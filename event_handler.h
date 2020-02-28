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
	QString id_;        // �û�id
	FileIo* fileIo_;    // �ļ�ģ��
	DataTransmitter* dataTransmitter_;  // ͨ��ģ��

public:
	EventHandler(QObject* parent = NULL);
	~EventHandler();
	void Init();
	// �û���ͬһIP��ַIDΨһ
	void Identify();

	// ��ȡ�û�id
	QString& GetId() { return id_; };
	// ����Ⱥ����Ϣ
	void SendSquareMsg(QString& msg);
	// �����»���
	void CreateTopic(QString& theme, QString& detail);
	// ɾ���ҵĻ���(index�ǻ��������л����е����)
	void DeleteTopic(int index);
	void DeleteTopic(QString& topicId);
	// ɾ�������ҵĻ���(myTopicIndex���ҵĻ��������л����ж�Ӧ������б�)
	void DeleteAllTopic(QVector<int>* myTopicIndex);
	void DeleteAllTopic(QString& userId);
	// ���ͻ�������
	void SendComment(QString& msg, QString& topicId);
	// ����������
	void CreateGroup(QString& name, QString& intro);
	// ����ĳ������(index�����������������е����)
	void EnterGroup(int index);
	void EnterGroup(QString& user_ip, QString& group_id);
	// �˳���ǰ����
	void ExitGroup(QString& GroupId);
	void ExitGroup(QString& user_ip, QString& group_id);
	// ɾ���ҵ�����(index�����������������е����)
	void DeleteGroup(int index);
	void DeleteGroup(QString& groupId);
	// ɾ�������ҵ�����(myGroupIndex���ҵ����������������ж�Ӧ������б�)
	void DeleteAllGroup(QVector<int>* myGroupIndex);
	void DeleteAllGroup(QString& userId);
	// ����������Ϣ
	void SendGroupMsg(QString& msg, QString& GroupId);

	// ��ȡ���л��⴫��ǰ��
	QJsonArray* GetAllTopic() { return fileIo_->GetTopic(); };
	// ��ȡ�������Ĵ���ǰ��
	QJsonArray* GetAllGroup() { return fileIo_->GetGroup(); };
	// ��ȡ������Ϣ����ǰ��
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

