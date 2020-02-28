#pragma execution_character_set("utf-8")
#ifndef _CHAT_UI_H_
#define _CHAT_UI_H_

#include <QObject>
#include <QString>
#include <QWidget>

#include "ui_chat.h"
#include "event_handler.h"

class ChatUi : public QWidget{
	Q_OBJECT
private:
	Ui::Form* ui_;
	EventHandler* eventHandler_;
	QString currentTopicId_;   // 当前正查看的话题的id
	QVector<int> myTopicIndex_;  // 我的话题在所有话题里一一对应的序号
	QString currentGroupId_;   // 当前正处于的组聊的id
	QVector<int> myGroupIndex_;  // 我的组聊在所有组聊里一一对应的序号

public:
	ChatUi(QWidget* parent = NULL);
	~ChatUi();

	// 关联前端信号
	void Init();

signals:

private slots:
	// 点击界面标签刷新当页内容
	void TabBarClicked(int index);
	// 发送广场消息
	void SendSquareMsg();
	// 创建话题
	void CreateTopic();
	// 选择某个具体话题
	void SelectTopic(int row);
	// 查看某个具体话题
	void CheckTopic();
	// 删除我的某个话题
	void DeleteTopic();
	// 删除我的所有话题
	void DeleteAllTopic();
	// 话题详情页的评论
	void SendComment();
	// 创建组聊
	void CreateGroup();
	// 选择某个具体组聊
	void SelectGroup(int row);
	// 进入某个具体组聊
	void EnterGroup();
	// 删除我的某个组聊
	void DeleteGroup();
	// 删除我的所有组聊
	void DeleteAllGroup();
	// 发送组聊消息
	void SendGroupMsg();
	// 退出某个组聊
	void ExitGroup();
public slots:
	// 收到广场消息
	void ReceiveSquareMsg(QJsonObject& msg);
	// ……
	void ReceiveNewTopicMsg(QJsonObject& msg);
	void ReceiveCommentMsg(QJsonObject& msg);
	void ReceiveDeleteTopicMsg();
	void ReceiveNewGroupMsg(QJsonObject& msg);
	void ReceiveGroupMsg(QJsonObject& msg);
	void ReceiveDeleteGroupMsg();
};

#endif // !_CHAT_UI_H_

