// 只管发送接收数据包，不考虑数据包内容（上层考虑）
#ifndef _DATA_TRANSMITTER_H_
#define _DATA_TRANSMITTER_H_

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTcpServer>
#include <QtCore>
#include <QByteArray>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <map>
#include <windows.h>
#include <QString>
#include <iostream>

class DataTransmitter : public QObject{
	Q_OBJECT
private:
	QString ip_;   // 用户自己的ip
	// UDP
	QUdpSocket* sendUdpSocket_;   // 专管发送的udpsocket
	QUdpSocket* recvUdpSocket_;   // 专管接收的udpsocket
	const qint16 sendUdpPort_ = 6666;
	const qint16 recvUdpPort_ = 7777;
	bool IsSelf(QString& ip);     // Udp广播是否是自己发的

	// TCP
	QTcpSocket* sendTcpSocket_;   // 专管发送的tcpsocket
	struct TcpReceiver{   // Tcp处理接收的一个连接
		QTcpSocket* socket;    // Tcp某连接套接字
		QByteArray* dataBuf;   // Tcp某连接接收数据的缓冲
		int dataSize;          // Tcp某连接应该收到的数据大小
	};
	std::map<qintptr, TcpReceiver*> tcpReceiver_; // tcp处理接收的一系列连接
	const qint16 sendTcpPort_ = 8888;
	const qint16 recvTcpPort_ = 9999;
	QTcpServer* tcpServer_;
	static const int bufSize_ = 1024;  // 按此大小的块接收发送
	void NewListen();   // 监听端口

protected:
	// 重写，tcpServer有连接到来自动调用，若不超过最大连接数
	// 创建并记录新的连接
	virtual void incomingConnection(qintptr socketHandle);

public:
	DataTransmitter(QObject* parent = 0);
	~DataTransmitter();
	void Init();  // 用于connect

	// 获取用户自己的ip地址
	QString GetIp() {
		return ip_;
	};
	void PrintIp();

	/*
	功能说明：Udp点对点发送
	传入参数：data - 需要传输的数据包、receiverIp - 传输目标Ip地址
	*/
	void UdpSendP2P(const QByteArray& data, const QString& receiverIp);

	/*
	功能说明：Udp局域网内广播发送
	传入参数：data - 需要传输的数据包
	*/
	void UdpSendBroadcast(const QByteArray& data);

	/*
	功能说明：Tcp点对点发送
	传入参数：data - 需要传输的数据包、receiverIp - 传输目标Ip地址
	*/
	void TcpSendP2P(const QByteArray& data, const QString& receiverIp);

signals:
	void TcpReadyRead(qintptr socketHandle);

	/*
	功能说明：Udp有数据到来时发出相应信号
	传出参数：data - 接收的数据包、senderIp - 传输来源Ip地址
	*/
	void UdpReceive(QByteArray* data, QString* senderIp);
	/*
	功能说明：Tcp有数据到来时发出相应信号
	传出参数：data - 接收的数据包
	*/
	void TcpReceive(QByteArray* data);


public slots:
	void UdpReadyReadSlot();     // Udp接收内容
	void TcpReadyReadSlot(qintptr socketHandle);     // Tcp接收内容

};

#endif // !_DATA_TRANSMITTER_H_