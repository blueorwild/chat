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
	QString currentTopicId_;   // ��ǰ���鿴�Ļ����id
	QVector<int> myTopicIndex_;  // �ҵĻ��������л�����һһ��Ӧ�����
	QString currentGroupId_;   // ��ǰ�����ڵ����ĵ�id
	QVector<int> myGroupIndex_;  // �ҵ�����������������һһ��Ӧ�����

public:
	ChatUi(QWidget* parent = NULL);
	~ChatUi();

	// ����ǰ���ź�
	void Init();

signals:

private slots:
	// ��������ǩˢ�µ�ҳ����
	void TabBarClicked(int index);
	// ���͹㳡��Ϣ
	void SendSquareMsg();
	// ��������
	void CreateTopic();
	// ѡ��ĳ�����廰��
	void SelectTopic(int row);
	// �鿴ĳ�����廰��
	void CheckTopic();
	// ɾ���ҵ�ĳ������
	void DeleteTopic();
	// ɾ���ҵ����л���
	void DeleteAllTopic();
	// ��������ҳ������
	void SendComment();
	// ��������
	void CreateGroup();
	// ѡ��ĳ����������
	void SelectGroup(int row);
	// ����ĳ����������
	void EnterGroup();
	// ɾ���ҵ�ĳ������
	void DeleteGroup();
	// ɾ���ҵ���������
	void DeleteAllGroup();
	// ����������Ϣ
	void SendGroupMsg();
	// �˳�ĳ������
	void ExitGroup();
public slots:
	// �յ��㳡��Ϣ
	void ReceiveSquareMsg(QJsonObject& msg);
	// ����
	void ReceiveNewTopicMsg(QJsonObject& msg);
	void ReceiveCommentMsg(QJsonObject& msg);
	void ReceiveDeleteTopicMsg();
	void ReceiveNewGroupMsg(QJsonObject& msg);
	void ReceiveGroupMsg(QJsonObject& msg);
	void ReceiveDeleteGroupMsg();
};

#endif // !_CHAT_UI_H_

