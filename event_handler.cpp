#include "event_handler.h"

EventHandler::EventHandler(QObject* parent) : QObject(parent) {
	fileIo_ = new FileIo();
	dataTransmitter_ = new DataTransmitter();
	dataTransmitter_->Init();
	Identify();
	//dataTransmitter_->PrintIp();
}

EventHandler::~EventHandler() {
	delete fileIo_;
	delete dataTransmitter_;
}

void EventHandler::Identify() {
	QString ip = dataTransmitter_->GetIp();
	QJsonArray* user = fileIo_->GetUser();
	for (auto& i : *user)
		if (i.toObject().value("ip").toString() == ip) {
			id_ = i.toObject().value("id").toString();
			return;
		}
	id_ = GenerateId(USER_ID_LEN);
	user->append(QJsonObject({ {"id", id_ },{"ip", ip} }));
}

void EventHandler::Init() {
	connect(dataTransmitter_, SIGNAL(UdpReceive(QByteArray*, QString*)), 
		this, SLOT(ReceiveUdpMsg(QByteArray*, QString*)));
}

void EventHandler::ReceiveUdpMsg(QByteArray* data, QString* senderIp) {
	QJsonObject msg(QJsonDocument::fromJson(*data).object());
	int type = msg.value("type").toInt();
	switch (type) {
	case SQUARE_MSG:
		//fileIo_->AddMessage(msg);
		emit ReceiveSquareMsg(msg);
		break;
	case NEW_TOPIC_MSG:
		fileIo_->AddTopic(msg);
		emit ReceiveNewTopicMsg(msg);
		break;
	case COMMENT_MSG:
		fileIo_->AddMessage(msg);
		emit ReceiveCommentMsg(msg);
		break;
	case DELETE_TOPIC_MSG:
		DeleteTopic(msg.value("topic_id").toString());
		emit ReceiveDeleteTopicMsg();
		break;
	case DELETE_ALL_TOPIC_MSG:
		DeleteAllTopic(msg.value("user_id").toString());
		emit ReceiveDeleteTopicMsg();
		break;
	case NEW_GROUP_MSG:
		fileIo_->AddGroup(msg);
		emit ReceiveNewGroupMsg(msg);
		break;
	case GROUP_MSG:
		fileIo_->AddMessage(msg);
		emit ReceiveGroupMsg(msg);
		break;
	case ENTER_GROUP_MSG:
		EnterGroup(*senderIp, msg.value("group_id").toString());
		break;
	case EXIT_GROUP_MSG:
		ExitGroup(*senderIp, msg.value("group_id").toString());
		break;
	case DELETE_GROUP_MSG:
		DeleteGroup(msg.value("group_id").toString());
		emit ReceiveDeleteGroupMsg();
		break;
	case DELETE_ALL_GROUP_MSG:
		DeleteAllGroup(msg.value("user_id").toString());
		emit ReceiveDeleteGroupMsg();
		break;
	}
}

void EventHandler::SendSquareMsg(QString &msg) {
	QJsonObject squareMsg({ {"type", SQUARE_MSG}, {"id", GenerateId(MESSAGE_ID_LEN) }, {"data", msg} });
	dataTransmitter_->UdpSendBroadcast(QJsonDocument(squareMsg).toJson());
	//fileIo_->AddMessage(squareMsg);
}

void EventHandler::CreateTopic(QString& theme, QString& detail) {
	QJsonObject newTopicMsg({ {"type", NEW_TOPIC_MSG} , {"id", GenerateId(TOPIC_ID_LEN) },
		{"user_id",id_},{"theme", theme},{"detail",detail} });
	dataTransmitter_->UdpSendBroadcast(QJsonDocument(newTopicMsg).toJson());
	fileIo_->AddTopic(newTopicMsg);
}
void EventHandler::DeleteTopic(int index) {
	// 话题和评论均删除
	QJsonArray* topic = fileIo_->GetTopic();
	QJsonArray* msg = fileIo_->GetMsg();
	QString topicId = topic->at(index).toObject().value("id").toString();
	topic->removeAt(index); // 删除话题

	// 删除评论(倒着删)
	for (int i = msg->size() - 1; i != -1; --i)
		if (msg->at(i).toObject().value("type") == COMMENT_MSG &&
			msg->at(i).toObject().value("type_id") == topicId) msg->removeAt(i);

	// 发送删除话题的消息
	QJsonObject deleteTopicMsg({ {"type", DELETE_TOPIC_MSG} , {"topic_id", topicId } });
	dataTransmitter_->UdpSendBroadcast(QJsonDocument(deleteTopicMsg).toJson());
}
void EventHandler::DeleteTopic(QString& topicId) {
	// 话题和评论均删除
	QJsonArray* topic = fileIo_->GetTopic();
	QJsonArray* msg = fileIo_->GetMsg();
	// 删除话题
	for (auto i = topic->begin(); i != topic->end(); ++i)
		if (i->toObject().value("id").toString() == topicId) topic->erase(i);
	// 删除评论(倒着删)
	for (int i = msg->size() - 1; i != -1; --i)
		if (msg->at(i).toObject().value("type") == COMMENT_MSG &&
			msg->at(i).toObject().value("type_id") == topicId) msg->removeAt(i);
}
void EventHandler::DeleteAllTopic(QVector<int>* myTopicIndex) {
	QJsonArray* topic = fileIo_->GetTopic();
	QJsonArray* msg = fileIo_->GetMsg();
	QStringList topicIdList;   // 保存所有的我的话题的id
	for (int i = myTopicIndex->size() - 1; i != -1; --i){  // 删除话题
		int index = myTopicIndex->at(i);
		topicIdList.append(topic->at(index).toObject().value("id").toString());
		topic->removeAt(index);
	}
	for (int i = msg->size() - 1; i != -1; --i)   // 删除评论
		if (msg->at(i).toObject().value("type") == COMMENT_MSG &&
			topicIdList.contains(msg->at(i).toObject().value("type_id").toString())) 
			msg->removeAt(i);

	// 发送删除所有我的话题的消息
	QJsonObject deleteAllTopicMsg({ {"type", DELETE_ALL_TOPIC_MSG} , {"user_id", id_ } });
	dataTransmitter_->UdpSendBroadcast(QJsonDocument(deleteAllTopicMsg).toJson());
}
void EventHandler::DeleteAllTopic(QString& userId) {
	QJsonArray* topic = fileIo_->GetTopic();
	QJsonArray* msg = fileIo_->GetMsg();
	QStringList topicIdList;
	for (int i = topic->size() - 1; i != -1; --i)  // 删除话题
		if (topic->at(i).toObject().value("user_id").toString() == userId) {
			topicIdList.append(topic->at(i).toObject().value("id").toString());
			topic->removeAt(i);
		}
	for (int i = msg->size() - 1; i != -1; --i)   // 删除评论
		if (msg->at(i).toObject().value("type") == COMMENT_MSG &&
			topicIdList.contains(msg->at(i).toObject().value("type_id").toString()))
			msg->removeAt(i);
}
void EventHandler::SendComment(QString& msg, QString& topicId) {
	QJsonObject commentMsg({ {"type", COMMENT_MSG} , {"id", GenerateId(MESSAGE_ID_LEN) },
		{"user_id",id_},{"data", msg},{"type_id",topicId } });
	dataTransmitter_->UdpSendBroadcast(QJsonDocument(commentMsg).toJson());
	fileIo_->AddMessage(commentMsg);
}

void EventHandler::CreateGroup(QString& name, QString& intro) {
	QJsonArray memberIpList;
	memberIpList.append(dataTransmitter_->GetIp());
	QJsonObject newGroupMsg({ {"type", NEW_GROUP_MSG} , {"id", GenerateId(GROUP_ID_LEN) },
		{"user_id",id_},{"name", name},{"intro",intro}, {"member_ip_list", memberIpList} });
	dataTransmitter_->UdpSendBroadcast(QJsonDocument(newGroupMsg).toJson());
	fileIo_->AddGroup(newGroupMsg);
}
void EventHandler::EnterGroup(int index) {
	// 排除是自己创建的组聊
	QJsonArray* group = fileIo_->GetGroup();
	QJsonObject groupObj = group->at(index).toObject();
	QString user_id = groupObj.value("user_id").toString();
	if (user_id == id_) return;
	else {  // 把我的ip加入组聊的ip列表
		QJsonArray memberIpList = groupObj.value("member_ip_list").toArray();
		memberIpList.append(dataTransmitter_->GetIp());
		QString id = groupObj.value("id").toString();
		QString name = groupObj.value("name").toString();
		QString intro = groupObj.value("intro").toString();
		QJsonObject newGroupMsg({ {"type", NEW_GROUP_MSG} , {"id", id },{"user_id",user_id},
			{"name", name},{"intro",intro}, {"member_ip_list", memberIpList} });
		group->replace(index, newGroupMsg);

		QJsonObject enterGroupMsg({ {"type", ENTER_GROUP_MSG},{"group_id", id} });
		dataTransmitter_->UdpSendBroadcast(QJsonDocument(enterGroupMsg).toJson());
	}
}
void EventHandler::EnterGroup(QString& userIp, QString& groupId) {
	QJsonArray* group = fileIo_->GetGroup();
	for (int i = 0; i < group->size(); ++i) // 把用户ip加入该组聊的ip列表
		if (group->at(i).toObject().value("id").toString() == groupId) {
			QJsonObject jsonObj = group->at(i).toObject();
			QJsonArray memberIpList = jsonObj.value("member_ip_list").toArray();
			memberIpList.append(userIp);
			QString userId = jsonObj.value("user_id").toString();
			QString name = jsonObj.value("name").toString();
			QString intro = jsonObj.value("intro").toString();
			QJsonObject newGroupMsg({ {"type", NEW_GROUP_MSG} , {"id", groupId },{"user_id",userId},
				{"name", name},{"intro",intro}, {"member_ip_list", memberIpList} });
			group->replace(i, newGroupMsg);
		}
}
void EventHandler::ExitGroup(QString& groupId) {
	// 若是自己创建的组聊，退出相当于解散
	QJsonArray* group = fileIo_->GetGroup();
	QJsonObject groupObj;
	for (int i = 0; i < group->size(); ++i) {
		if (group->at(i).toObject().value("id").toString() == groupId) {
			groupObj = group->at(i).toObject();
			QString user_id = groupObj.value("user_id").toString();
			if (user_id == id_) DeleteGroup(i);
			else {  // 把我的ip从组聊的ip列表删除
				QVector<QVariant> memberIpList = groupObj.value("member_ip_list").toArray().toVariantList().toVector();
				memberIpList.removeOne(dataTransmitter_->GetIp());
				QString name = groupObj.value("name").toString();
				QString intro = groupObj.value("intro").toString();
				QJsonObject newGroupMsg({ {"type", NEW_GROUP_MSG} , {"id", groupId},{"user_id", user_id},{"name", name},
					{"intro",intro}, {"member_ip_list", QJsonArray::fromVariantList(memberIpList.toList())} });
				group->replace(i, newGroupMsg);

				QJsonObject exitGroupMsg({ {"type", EXIT_GROUP_MSG},{"group_id", groupId} });
				dataTransmitter_->UdpSendBroadcast(QJsonDocument(exitGroupMsg).toJson());
			}
			return;
		}
	}
}
void EventHandler::ExitGroup(QString& userIp, QString& groupId) {
	QJsonArray* group = fileIo_->GetGroup();
	for (int i = 0; i < group->size(); ++i) // 把用户ip加入该组聊的ip列表
		if (group->at(i).toObject().value("id").toString() == groupId) {
			QJsonObject groupObj = group->at(i).toObject();
			QVector<QVariant> memberIpList = groupObj.value("member_ip_list").toArray().toVariantList().toVector();
			memberIpList.removeOne(userIp);
			QString name = groupObj.value("name").toString();
			QString intro = groupObj.value("intro").toString();
			QString user_id = groupObj.value("user_id").toString();
			QJsonObject newGroupMsg({ {"type", NEW_GROUP_MSG} , {"id", groupId},{"user_id", user_id},{"name", name},
				{"intro",intro}, {"member_ip_list", QJsonArray::fromVariantList(memberIpList.toList())} });
			group->replace(i, newGroupMsg);
		}
}
void EventHandler::DeleteGroup(int index) {
	// 组聊和组聊消息均删除
	QJsonArray* group = fileIo_->GetGroup();
	QJsonArray* msg = fileIo_->GetMsg();
	QString groupId = group->at(index).toObject().value("id").toString();
	group->removeAt(index); // 删除话题

	// 删除组聊消息(倒着删)
	for (int i = msg->size() - 1; i != -1; --i)
		if (msg->at(i).toObject().value("type") == GROUP_MSG &&
			msg->at(i).toObject().value("type_id") == groupId) msg->removeAt(i);

	// 发送删除组聊的消息
	QJsonObject deleteGroupMsg({ {"type", DELETE_GROUP_MSG} , {"group_id", groupId } });
	dataTransmitter_->UdpSendBroadcast(QJsonDocument(deleteGroupMsg).toJson());
}
void EventHandler::DeleteGroup(QString& groupId) {
	// 组聊和组聊消息均删除
	QJsonArray* group = fileIo_->GetGroup();
	QJsonArray* msg = fileIo_->GetMsg();
	// 删除组聊
	for (auto i = group->begin(); i != group->end(); ++i)
		if (i->toObject().value("id").toString() == groupId) group->erase(i);
	// 删除组聊消息(倒着删)
	for (int i = msg->size() - 1; i != -1; --i)
		if (msg->at(i).toObject().value("type") == GROUP_MSG &&
			msg->at(i).toObject().value("type_id") == groupId) msg->removeAt(i);
}
void EventHandler::DeleteAllGroup(QVector<int>* myGroupIndex) {
	QJsonArray* group = fileIo_->GetGroup();
	QJsonArray* msg = fileIo_->GetMsg();
	QStringList groupIdList;   // 保存所有的我的组聊的id
	for (int i = myGroupIndex->size() - 1; i != -1; --i) {  // 删除组聊
		int index = myGroupIndex->at(i);
		groupIdList.append(group->at(index).toObject().value("id").toString());
		group->removeAt(index);
	}
	for (int i = msg->size() - 1; i != -1; --i)   // 删除组聊消息
		if (msg->at(i).toObject().value("type") == GROUP_MSG &&
			groupIdList.contains(msg->at(i).toObject().value("type_id").toString()))
			msg->removeAt(i);

	// 发送删除所有我的组聊的消息
	QJsonObject deleteAllGroupMsg({ {"type", DELETE_ALL_GROUP_MSG} , {"user_id", id_ } });
	dataTransmitter_->UdpSendBroadcast(QJsonDocument(deleteAllGroupMsg).toJson());
}
void EventHandler::DeleteAllGroup(QString& userId) {
	QJsonArray* group = fileIo_->GetGroup();
	QJsonArray* msg = fileIo_->GetMsg();
	QStringList groupIdList;
	for (int i = group->size() - 1; i != -1; --i)  // 删除话题
		if (group->at(i).toObject().value("user_id").toString() == userId) {
			groupIdList.append(group->at(i).toObject().value("id").toString());
			group->removeAt(i);
		}
	for (int i = msg->size() - 1; i != -1; --i)   // 删除评论
		if (msg->at(i).toObject().value("type") == GROUP_MSG &&
			groupIdList.contains(msg->at(i).toObject().value("type_id").toString()))
			msg->removeAt(i);
}
void EventHandler::SendGroupMsg(QString& msg, QString& groupId) {
	QJsonObject groupMsg({ {"type", GROUP_MSG} , {"id", GenerateId(MESSAGE_ID_LEN) },
		{"user_id",id_},{"data", msg},{"type_id",groupId } });
	QJsonArray* group = fileIo_->GetGroup();
	QJsonArray memberIpList;
	for (auto& i : *group)
		if (i.toObject().value("id").toString() == groupId)
			memberIpList = i.toObject().value("member_ip_list").toArray();
	for (auto& i : memberIpList)
		dataTransmitter_->UdpSendP2P(QJsonDocument(groupMsg).toJson(), i.toString());
	fileIo_->AddMessage(groupMsg);
}
