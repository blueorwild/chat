#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

#include <QObject>
#include <iostream>
#include <QVector>
#include <QVariant>

#include "file_io.h"
#include "data_transmitter.h"

class EventHandler : public QObject{
	Q_OBJECT
private:
	QString id_;        // 用户id
	FileIo* fileIo_;    // 文件模块
	DataTransmitter* dataTransmitter_;  // 通信模块

public:
	EventHandler(QObject* parent = NULL);
	~EventHandler();
	void Init();
	// 用户在同一IP地址ID唯一
	void Identify();

	// 获取用户id
	QString& GetId() { return id_; };
	// 发送群聊消息
	void SendSquareMsg(QString& msg);
	// 创建新话题
	void CreateTopic(QString& theme, QString& detail);
	// 删除我的话题(index是话题在所有话题中的序号)
	void DeleteTopic(int index);
	void DeleteTopic(QString& topicId);
	// 删除所有我的话题(myTopicIndex是我的话题在所有话题中对应的序号列表)
	void DeleteAllTopic(QVector<int>* myTopicIndex);
	void DeleteAllTopic(QString& userId);
	// 发送话题评论
	void SendComment(QString& msg, QString& topicId);
	// 创建新组聊
	void CreateGroup(QString& name, QString& intro);
	// 加入某个组聊(index是组聊在所有组聊中的序号)
	void EnterGroup(int index);
	void EnterGroup(QString& user_ip, QString& group_id);
	// 退出当前组聊
	void ExitGroup(QString& GroupId);
	void ExitGroup(QString& user_ip, QString& group_id);
	// 删除我的组聊(index是组聊在所有组聊中的序号)
	void DeleteGroup(int index);
	void DeleteGroup(QString& groupId);
	// 删除所有我的组聊(myGroupIndex是我的组聊在所有组聊中对应的序号列表)
	void DeleteAllGroup(QVector<int>* myGroupIndex);
	void DeleteAllGroup(QString& userId);
	// 发送组聊消息
	void SendGroupMsg(QString& msg, QString& GroupId);

	// 获取所有话题传到前端
	QJsonArray* GetAllTopic() { return fileIo_->GetTopic(); };
	// 获取所有组聊传到前端
	QJsonArray* GetAllGroup() { return fileIo_->GetGroup(); };
	// 获取所有消息传到前端
	QJsonArray* GetAllMessage() { return fileIo_->GetMsg(); };

signals:
	void ReceiveSquareMsg(QJsonObject &msg);
	void ReceiveNewTopicMsg(QJsonObject& msg);
	void ReceiveCommentMsg(QJsonObject& msg);
	void ReceiveDeleteTopicMsg();
	void ReceiveNewGroupMsg(QJsonObject& msg);
	void ReceiveGroupMsg(QJsonObject& msg);
	void ReceiveDeleteGroupMsg();


public slots:
	void ReceiveUdpMsg(QByteArray* data, QString* senderIp);

};


#endif // !_EVENT_HANDLER_H_

