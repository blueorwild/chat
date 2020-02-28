// ֻ�ܷ��ͽ������ݰ������������ݰ����ݣ��ϲ㿼�ǣ�
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
	QString ip_;   // �û��Լ���ip
	// UDP
	QUdpSocket* sendUdpSocket_;   // ר�ܷ��͵�udpsocket
	QUdpSocket* recvUdpSocket_;   // ר�ܽ��յ�udpsocket
	const qint16 sendUdpPort_ = 6666;
	const qint16 recvUdpPort_ = 7777;
	bool IsSelf(QString& ip);     // Udp�㲥�Ƿ����Լ�����

	// TCP
	QTcpSocket* sendTcpSocket_;   // ר�ܷ��͵�tcpsocket
	struct TcpReceiver{   // Tcp������յ�һ������
		QTcpSocket* socket;    // Tcpĳ�����׽���
		QByteArray* dataBuf;   // Tcpĳ���ӽ������ݵĻ���
		int dataSize;          // Tcpĳ����Ӧ���յ������ݴ�С
	};
	std::map<qintptr, TcpReceiver*> tcpReceiver_; // tcp������յ�һϵ������
	const qint16 sendTcpPort_ = 8888;
	const qint16 recvTcpPort_ = 9999;
	QTcpServer* tcpServer_;
	static const int bufSize_ = 1024;  // ���˴�С�Ŀ���շ���
	void NewListen();   // �����˿�

protected:
	// ��д��tcpServer�����ӵ����Զ����ã������������������
	// ��������¼�µ�����
	virtual void incomingConnection(qintptr socketHandle);

public:
	DataTransmitter(QObject* parent = 0);
	~DataTransmitter();
	void Init();  // ����connect

	// ��ȡ�û��Լ���ip��ַ
	QString GetIp() {
		return ip_;
	};
	void PrintIp();

	/*
	����˵����Udp��Ե㷢��
	���������data - ��Ҫ��������ݰ���receiverIp - ����Ŀ��Ip��ַ
	*/
	void UdpSendP2P(const QByteArray& data, const QString& receiverIp);

	/*
	����˵����Udp�������ڹ㲥����
	���������data - ��Ҫ��������ݰ�
	*/
	void UdpSendBroadcast(const QByteArray& data);

	/*
	����˵����Tcp��Ե㷢��
	���������data - ��Ҫ��������ݰ���receiverIp - ����Ŀ��Ip��ַ
	*/
	void TcpSendP2P(const QByteArray& data, const QString& receiverIp);

signals:
	void TcpReadyRead(qintptr socketHandle);

	/*
	����˵����Udp�����ݵ���ʱ������Ӧ�ź�
	����������data - ���յ����ݰ���senderIp - ������ԴIp��ַ
	*/
	void UdpReceive(QByteArray* data, QString* senderIp);
	/*
	����˵����Tcp�����ݵ���ʱ������Ӧ�ź�
	����������data - ���յ����ݰ�
	*/
	void TcpReceive(QByteArray* data);


public slots:
	void UdpReadyReadSlot();     // Udp��������
	void TcpReadyReadSlot(qintptr socketHandle);     // Tcp��������

};

#endif // !_DATA_TRANSMITTER_H_