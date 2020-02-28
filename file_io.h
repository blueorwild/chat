// 主要针对json文件
#ifndef _FILE_IO_H_
#define _FINE_IO_H_

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

#include "public.h"

class FileIo {
private:
	// 四张表
	QJsonArray user_;
	QJsonArray topic_;
	QJsonArray group_;
	QJsonArray message_;

public:
	FileIo();
	~FileIo();

	void AddUser(QJsonObject& user) { user_.append(user); };
	void AddTopic(QJsonObject& topic) { topic_.append(topic); };
	void AddGroup(QJsonObject& group) { group_.append(group); };
	void AddMessage(QJsonObject &msg) { message_.append(msg); };

	QJsonArray* GetMsg() { return &message_; }
	QJsonArray* GetUser() { return &user_; }
	QJsonArray* GetTopic() { return &topic_; }
	QJsonArray* GetGroup() { return &group_; }

	void SaveFile(QString& fileName, QJsonArray& jsonArray);
	void LoadFile(QString &fileName, QJsonArray &jsonArray);
};


#endif // !_FILE_IO_H_

