#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#include <QString>

enum MessageType
{
	SQUARE_MSG, NEW_TOPIC_MSG, COMMENT_MSG, DELETE_TOPIC_MSG, DELETE_ALL_TOPIC_MSG,
	NEW_GROUP_MSG, GROUP_MSG, DELETE_GROUP_MSG, DELETE_ALL_GROUP_MSG, ENTER_GROUP_MSG,
	EXIT_GROUP_MSG,
};


const int USER_ID_LEN = 12;
const int TOPIC_ID_LEN = 8;
const int GROUP_ID_LEN = 8;
const int MESSAGE_ID_LEN = 16;

const QString FILE_DIR = "./data/";

// 产生字节数为digits的随机id
QString GenerateId(int digits);

#endif // !_PUBLIC_H_
