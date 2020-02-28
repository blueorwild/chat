#include <iostream>
#include "chat_ui.h"
#include <QApplication>
using namespace std;

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	ChatUi chat;
	chat.Init();
	chat.show();
	return app.exec();
}

