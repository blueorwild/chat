#include "file_io.h"

FileIo::FileIo() {
	LoadFile(QString("user.txt"), user_);
	LoadFile(QString("topic.txt"), topic_);
	LoadFile(QString("group.txt"), group_);
	LoadFile(QString("message.txt"), message_);
}

FileIo::~FileIo() {
	SaveFile(QString("user.txt"), user_);
	SaveFile(QString("topic.txt"), topic_);
	SaveFile(QString("group.txt"), group_);
	SaveFile(QString("message.txt"), message_);
}

void FileIo::SaveFile(QString& fileName, QJsonArray &jsonArray) {
	QFile file(FILE_DIR + fileName);
	file.open(QIODevice::WriteOnly);
	file.write(QJsonDocument(jsonArray).toJson());
	file.close();
}
void FileIo::LoadFile(QString &fileName, QJsonArray &jsonArray) {
	QFile file(FILE_DIR + fileName);
	if (!file.open(QIODevice::ReadOnly)) return;
	jsonArray = QJsonDocument::fromJson(file.readAll()).array();
	file.close();
}