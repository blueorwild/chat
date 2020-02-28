#include "chat_ui.h"
#include <iostream>

ChatUi::ChatUi(QWidget* parent) : QWidget(parent) {
	ui_ = new Ui::Form();
	ui_->setupUi(this);
	eventHandler_ = new EventHandler;
	eventHandler_->Init();
	ui_->tabWidget->tabBar()->setTabEnabled(4, false);  // ���������顰���ɵ��
	ui_->tabWidget->tabBar()->setTabEnabled(8, false);  // ���������顰���ɵ��
}

ChatUi::~ChatUi() {
	delete ui_;
	delete eventHandler_;
}

void ChatUi::Init() {
	connect(ui_->tabWidget, SIGNAL(tabBarClicked(int)), this, SLOT(TabBarClicked(int)));
	connect(ui_->pushButton, SIGNAL(clicked()), this, SLOT(SendSquareMsg()));
	connect(ui_->pushButton_2, SIGNAL(clicked()), this, SLOT(CreateTopic()));
	connect(ui_->listWidget_2, SIGNAL(currentRowChanged(int)), this, SLOT(SelectTopic(int)));
	connect(ui_->pushButton_3, SIGNAL(clicked()), this, SLOT(CheckTopic()));
	connect(ui_->listWidget_4, SIGNAL(currentRowChanged(int)), this, SLOT(SelectTopic(int)));
	connect(ui_->pushButton_5, SIGNAL(clicked()), this, SLOT(CheckTopic()));
	connect(ui_->pushButton_6, SIGNAL(clicked()), this, SLOT(DeleteTopic()));
	connect(ui_->pushButton_7, SIGNAL(clicked()), this, SLOT(DeleteAllTopic()));
	connect(ui_->pushButton_4, SIGNAL(clicked()), this, SLOT(SendComment()));
	connect(ui_->pushButton_8, SIGNAL(clicked()), this, SLOT(CreateGroup()));
	connect(ui_->listWidget_5, SIGNAL(currentRowChanged(int)), this, SLOT(SelectGroup(int)));
	connect(ui_->pushButton_9, SIGNAL(clicked()), this, SLOT(EnterGroup()));
	connect(ui_->listWidget_6, SIGNAL(currentRowChanged(int)), this, SLOT(SelectGroup(int)));
	connect(ui_->pushButton_10, SIGNAL(clicked()), this, SLOT(EnterGroup()));
	connect(ui_->pushButton_11, SIGNAL(clicked()), this, SLOT(DeleteGroup()));
	connect(ui_->pushButton_12, SIGNAL(clicked()), this, SLOT(DeleteAllGroup()));
	connect(ui_->pushButton_13, SIGNAL(clicked()), this, SLOT(SendGroupMsg()));
	connect(ui_->pushButton_14, SIGNAL(clicked()), this, SLOT(ExitGroup()));

	connect(eventHandler_, SIGNAL(ReceiveSquareMsg(QJsonObject&)), this, SLOT(ReceiveSquareMsg(QJsonObject&)));
	connect(eventHandler_, SIGNAL(ReceiveNewTopicMsg(QJsonObject&)), this, SLOT(ReceiveNewTopicMsg(QJsonObject&)));
	connect(eventHandler_, SIGNAL(ReceiveCommentMsg(QJsonObject&)), this, SLOT(ReceiveCommentMsg(QJsonObject&)));
	connect(eventHandler_, SIGNAL(ReceiveDeleteTopicMsg()), this, SLOT(ReceiveDeleteTopicMsg()));
	connect(eventHandler_, SIGNAL(ReceiveNewGroupMsg(QJsonObject&)), this, SLOT(ReceiveNewGroupMsg(QJsonObject&)));
	connect(eventHandler_, SIGNAL(ReceiveGroupMsg(QJsonObject&)), this, SLOT(ReceiveGroupMsg(QJsonObject&)));
	connect(eventHandler_, SIGNAL(ReceiveDeleteGroupMsg()), this, SLOT(ReceiveDeleteGroupMsg()));
}

void ChatUi::TabBarClicked(int index) {
	switch (index) {
	case 1: {  // ��������
		ui_->lineEdit->clear();
		ui_->textEdit_2->clear();
		break;
	}
	case 2: {  // �鿴���л���
		ui_->pushButton_3->setEnabled(false);
		ui_->listWidget_2->clear();
		QJsonArray* topic = eventHandler_->GetAllTopic();
		for (auto& i : *topic) ui_->listWidget_2->addItem(i.toObject().value("theme").toString());
		break;
	}
	case 3: {  // �鿴�ҵĻ���
		ui_->pushButton_5->setEnabled(false);
		ui_->pushButton_6->setEnabled(false);
		ui_->listWidget_4->clear();
		myTopicIndex_.clear();
		QJsonArray* topic = eventHandler_->GetAllTopic();
		QString userId = eventHandler_->GetId();
		int index = 0;
		for (auto& i : *topic)
			if (i.toObject().value("user_id").toString() == userId) {
				ui_->listWidget_4->addItem(i.toObject().value("theme").toString());
				myTopicIndex_.append(index++);
			}
		break;
	}
	case 5: {  // ��������
		ui_->lineEdit_2->clear();
		ui_->textEdit_4->clear();
		break;
	}
	case 6: {  // �鿴��������
		ui_->pushButton_9->setEnabled(false);
		ui_->listWidget_5->clear();
		QJsonArray* group = eventHandler_->GetAllGroup();
		for (auto& i : *group) ui_->listWidget_5->addItem(i.toObject().value("name").toString());
		break;
	}
	case 7: {  // �鿴�ҵ�����
		ui_->pushButton_10->setEnabled(false);
		ui_->pushButton_11->setEnabled(false);
		ui_->listWidget_6->clear();
		myGroupIndex_.clear();
		QJsonArray* group = eventHandler_->GetAllGroup();
		QString userId = eventHandler_->GetId();
		int index = 0;
		for (auto& i : *group)
			if (i.toObject().value("user_id").toString() == userId) {
				ui_->listWidget_6->addItem(i.toObject().value("name").toString());
				myGroupIndex_.append(index++);
			}
		break;
	}
	default:
		break;
	}
}

void ChatUi::SendSquareMsg() {
	QString msg = ui_->textEdit->toPlainText();
	if (msg.isEmpty()) return;
	eventHandler_->SendSquareMsg(msg);

	ui_->textEdit->clear();
	ui_->listWidget->addItem(msg);
}

void ChatUi::CreateTopic() {
	QString theme = ui_->lineEdit->text();
	if (theme.isEmpty()) return;
	QString detail = ui_->textEdit_2->toPlainText();
	if (detail.isEmpty()) return;
	eventHandler_->CreateTopic(theme, detail);
	// ת���ҵĻ���ҳ��
	ui_->tabWidget->setCurrentIndex(3);
	TabBarClicked(3);
}

void ChatUi::SelectTopic(int row) {
	// ���������л���ҳ�滹���ҵĻ���ҳ��
	if (ui_->tabWidget->currentIndex() == 2) {  // ���л���
		ui_->listWidget_2->setCurrentRow(row);
		if (row == -1) ui_->pushButton_3->setEnabled(false);
		else ui_->pushButton_3->setEnabled(true);
	}
	else if (ui_->tabWidget->currentIndex() == 3) {  // �ҵĻ���
		ui_->listWidget_4->setCurrentRow(row);
		if (row == -1) {
			ui_->pushButton_5->setEnabled(false);
			ui_->pushButton_6->setEnabled(false);
		}
		else {
			ui_->pushButton_5->setEnabled(true);
			ui_->pushButton_6->setEnabled(true);
		}
	}
}

void ChatUi::CheckTopic() {
	// ͬ�����������л���ҳ�滹���ҵĻ���ҳ��
	int currentIndex = ui_->tabWidget->currentIndex();
	ui_->tabWidget->setCurrentIndex(4);   // ��������ҳ��
	ui_->textBrowser->clear();
	ui_->textBrowser_2->clear();
	ui_->listWidget_3->clear();

	QJsonArray* topic = eventHandler_->GetAllTopic();
	QJsonArray* msg = eventHandler_->GetAllMessage();
	QJsonObject topicObj;   // �鿴�Ļ���Ŀ��

	if (currentIndex == 2) // ���л���
		topicObj = topic->at(ui_->listWidget_2->currentRow()).toObject();
	else if (currentIndex == 3 && !myTopicIndex_.empty()) // �ҵĻ���
		topicObj = topic->at(myTopicIndex_.at(ui_->listWidget_4->currentRow())).toObject();

	// ��ȡ��չʾ����
	ui_->textBrowser->setPlainText(topicObj.value("theme").toString());
	ui_->textBrowser_2->setPlainText(topicObj.value("detail").toString());
	currentTopicId_ = topicObj.value("id").toString();
	// ��ȡ��չʾ����
	for (auto& i : *msg)
		if (i.toObject().value("type").toInt() == COMMENT_MSG &&
			i.toObject().value("type_id").toString() == currentTopicId_)
			ui_->listWidget_3->addItem(i.toObject().value("data").toString());

}

void ChatUi::DeleteTopic() {
	int row = ui_->listWidget_4->currentRow();
	if (row != -1) eventHandler_->DeleteTopic(myTopicIndex_.at(row));
	// ���µ�ǰҳ��
	TabBarClicked(3);
}

void ChatUi::DeleteAllTopic() {
	eventHandler_->DeleteAllTopic(&myTopicIndex_);
	// ���µ�ǰҳ��
	ui_->listWidget_4->clear();
	myTopicIndex_.clear();
}

void ChatUi::SendComment() {
	QString msg = ui_->textEdit_3->toPlainText();
	if (msg.isEmpty()) return;
	eventHandler_->SendComment(msg, currentTopicId_);

	ui_->textEdit_3->clear();
	ui_->listWidget_3->addItem(msg);
}

void ChatUi::CreateGroup() {
	QString name = ui_->lineEdit_2->text();
	if (name.isEmpty()) return;
	QString intro = ui_->textEdit_4->toPlainText();
	if (intro.isEmpty()) return;
	eventHandler_->CreateGroup(name, intro);
	// ת���ҵ�����ҳ��
	ui_->tabWidget->setCurrentIndex(7);
	TabBarClicked(7);
}

void ChatUi::SelectGroup(int row) {
	// ��������������ҳ�滹���ҵ�����ҳ��
	if (ui_->tabWidget->currentIndex() == 6) {  // ���л���
		ui_->listWidget_5->setCurrentRow(row);
		if (row == -1) ui_->pushButton_9->setEnabled(false);
		else ui_->pushButton_9->setEnabled(true);
	}
	else if (ui_->tabWidget->currentIndex() == 7) {  // �ҵĻ���
		ui_->listWidget_6->setCurrentRow(row);
		if (row == -1) {
			ui_->pushButton_10->setEnabled(false);
			ui_->pushButton_11->setEnabled(false);
		}
		else {
			ui_->pushButton_10->setEnabled(true);
			ui_->pushButton_11->setEnabled(true);
		}
	}
}

void ChatUi::EnterGroup() {
	// ͬ����������������ҳ�滹���ҵ�����ҳ��
	int currentIndex = ui_->tabWidget->currentIndex();
	ui_->tabWidget->setCurrentIndex(8);   // ��������ҳ��
	ui_->textBrowser_3->clear();
	ui_->listWidget_7->clear();

	QJsonArray* group = eventHandler_->GetAllGroup();
	QJsonArray* msg = eventHandler_->GetAllMessage();
	QJsonObject groupObj;   // �鿴������Ŀ��

	if (currentIndex == 6) { // ��������
		groupObj = group->at(ui_->listWidget_5->currentRow()).toObject();
		eventHandler_->EnterGroup(ui_->listWidget_5->currentRow()); // ���ҵ�id���뵽��������
	}
	else if (currentIndex == 7 && !myGroupIndex_.empty())    // �ҵ�����
		groupObj = group->at(myGroupIndex_.at(ui_->listWidget_6->currentRow())).toObject();

	// ��ȡ��չʾ����
	ui_->textBrowser_3->setPlainText(groupObj.value("name").toString());
	currentGroupId_ = groupObj.value("id").toString();
	// ��ȡ��չʾ������Ϣ
	for (auto& i : *msg)
		if (i.toObject().value("type").toInt() == GROUP_MSG &&
			i.toObject().value("type_id").toString() == currentGroupId_)
			ui_->listWidget_7->addItem(i.toObject().value("data").toString());

}

void ChatUi::DeleteGroup() {
	int row = ui_->listWidget_6->currentRow();
	if (row != -1) eventHandler_->DeleteGroup(myGroupIndex_.at(row));
	// ���µ�ǰҳ��
	TabBarClicked(7);
}

void ChatUi::DeleteAllGroup() {
	eventHandler_->DeleteAllGroup(&myGroupIndex_);
	// ���µ�ǰҳ��
	ui_->listWidget_6->clear();
	myGroupIndex_.clear();
}

void ChatUi::SendGroupMsg() {
	QString msg = ui_->textEdit_5->toPlainText();
	if (msg.isEmpty()) return;
	eventHandler_->SendGroupMsg(msg, currentGroupId_);
	// ����ҳ��
	ui_->textEdit_5->clear();
	ui_->listWidget_7->addItem(msg);
}

void ChatUi::ExitGroup() {
	eventHandler_->ExitGroup(currentGroupId_);
	ui_->tabWidget->setCurrentIndex(6);   // ��������ҳ��
	TabBarClicked(6);
}

void ChatUi::ReceiveSquareMsg(QJsonObject& msg) {
	QString data = msg.value("data").toString();
	ui_->listWidget->addItem(data);
}
void ChatUi::ReceiveNewTopicMsg(QJsonObject& msg) {
	ui_->listWidget_2->addItem(msg.value("theme").toString());
	ui_->listWidget_4->addItem(msg.value("theme").toString());
}
void ChatUi::ReceiveCommentMsg(QJsonObject& msg) {
	ui_->listWidget_3->addItem(msg.value("data").toString());
}
void ChatUi::ReceiveDeleteTopicMsg() {
	TabBarClicked(2);
}
void ChatUi::ReceiveNewGroupMsg(QJsonObject& msg) {
	ui_->listWidget_5->addItem(msg.value("name").toString());
	ui_->listWidget_6->addItem(msg.value("name").toString());
}
void ChatUi::ReceiveGroupMsg(QJsonObject& msg) {
	ui_->listWidget_7->addItem(msg.value("data").toString());
}
void ChatUi::ReceiveDeleteGroupMsg() {
	TabBarClicked(6);
}