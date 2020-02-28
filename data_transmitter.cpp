#include "data_transmitter.h"
#include <assert.h>

DataTransmitter::DataTransmitter(QObject* parent):QObject(parent){
	// ��ȡIp--����ip�б�����һ������ʱ��ô�ɣ�
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	for (auto i = ipAddressesList.end() - 1; i != ipAddressesList.begin(); --i)
		if (*i != QHostAddress::LocalHost && (*i).toIPv4Address()) {
			ip_ = (*i).toString();
			break;
		}
	// UDP
	sendUdpSocket_ = new QUdpSocket;
	recvUdpSocket_ = new QUdpSocket;
	sendUdpSocket_->bind(QHostAddress(ip_), sendUdpPort_);
	recvUdpSocket_->bind(QHostAddress(ip_), recvUdpPort_);
	
	// TCP
	sendTcpSocket_ = new QTcpSocket;
	sendTcpSocket_->bind(QHostAddress(ip_), sendTcpPort_);
	tcpServer_ = new QTcpServer;
	NewListen();
};
DataTransmitter::~DataTransmitter() {
	delete sendUdpSocket_;
	delete recvUdpSocket_;
	delete sendTcpSocket_;
	delete tcpServer_;

	for (auto&i : tcpReceiver_) {
		delete i.second->socket;
		delete i.second->dataBuf;
	}
};

void DataTransmitter::Init() {
	connect(recvUdpSocket_, SIGNAL(readyRead()), this, SLOT(UdpReadyReadSlot()));
	connect(this, SIGNAL(TcpReadyRead(qintptr)), this, SLOT(TcpReadyReadSlot(qintptr)));
}

void DataTransmitter::NewListen() {
	if (!tcpServer_->listen(QHostAddress(ip_), recvTcpPort_)) {
		// ��Ҫ������־��ӡ������Ϣ
		std::cout << "TcpServer�����������˿�ʧ�ܣ�\n";
		tcpServer_->close();
	}
}

void DataTransmitter::incomingConnection(qintptr socketHandle) {
	QTcpSocket* newSocket = new QTcpSocket();
	newSocket->setSocketDescriptor(socketHandle);
	// ���������������
	if (tcpReceiver_.size() > tcpServer_->maxPendingConnections()) {
		newSocket->disconnectFromHost();
		delete newSocket;
		return;
	}
	// �����¼����
	tcpReceiver_[socketHandle]->socket = newSocket;
	tcpReceiver_[socketHandle]->dataBuf = new QByteArray;
	tcpReceiver_[socketHandle]->dataSize = 0;
	connect(newSocket, &QTcpSocket::readyRead, [socketHandle, this]()-> \
		void{ emit TcpReadyRead(socketHandle); });
}

void DataTransmitter::UdpSendP2P(const QByteArray &data, const QString &receiverIp) {
	sendUdpSocket_->writeDatagram(data, QHostAddress(receiverIp), recvUdpPort_);
}

void DataTransmitter::UdpSendBroadcast(const QByteArray &data) {
	sendUdpSocket_->writeDatagram(data, QHostAddress::Broadcast, recvUdpPort_);
}

void DataTransmitter::UdpReadyReadSlot() {
	QNetworkDatagram dataGram = recvUdpSocket_->receiveDatagram();
	QByteArray* data = new QByteArray(dataGram.data());
	QString* senderIp = new QString(dataGram.senderAddress().toString());
	// �Լ������ľ�����
	if(!IsSelf(*senderIp)) emit UdpReceive(data, senderIp);
}

void DataTransmitter::TcpSendP2P(const QByteArray &data, const QString &receiverIp) {
	// ע�⣬���п�С��ΪbufSize_,ÿ�����һ�ֽ��Ǳ�־λ��Ϊ��Ч�ʣ�
	// ������
	int tryTimes = 0;
TRY_CONNECT:
	sendTcpSocket_->connectToHost(QHostAddress(receiverIp), recvTcpPort_);
CONNECT_OK:
	if (sendTcpSocket_->state() == QTcpSocket::UnconnectedState) {
		if (++tryTimes == 3) {
			// ��¼��־
			return;
		}
		Sleep(1000);
		goto TRY_CONNECT;
	}

	// Ȼ��ͷ��:size
	int size = data.size();
	QByteArray head = QByteArray::number(size);
	head.resize(bufSize_);
	head[bufSize_ - 1] = 'h';  // ��������ͷ��
	if (bufSize_ != sendTcpSocket_->write(head)) {
		// ��¼��־
		goto CONNECT_OK;
	}; 

	// Ȼ������
	int leftSize = size;
	const char* p = data;
	while (leftSize >= bufSize_ - 1) {
		QByteArray content(p, bufSize_ - 1);
		content[bufSize_ - 1] = 'c'; // ������������
		leftSize -= (bufSize_ - 1);
		p += (bufSize_ - 1);
		if (bufSize_ != sendTcpSocket_->write(content)) {
			// ��¼��־
			goto CONNECT_OK;
		};
	}

	// ���������leftSize == 0��leftSize > 0
	if (bufSize_ > 0) {
		QByteArray content(p, leftSize);
		content.resize(bufSize_);
		content[bufSize_ - 1] = 'c';
		if (bufSize_ != sendTcpSocket_->write(content)) {
			// ��¼��־
			goto CONNECT_OK;
		};
	}

	// �������ʾ���ݷ�����ɣ��ٷ�һ��������־
	QByteArray content(bufSize_, '\0');
	content[bufSize_ - 1] = 'e';
	if (bufSize_ != sendTcpSocket_->write(content)) {
		// ��¼��־
		goto CONNECT_OK;
	};

	// ��ɴ˴δ��䣬�Ͽ�����
	sendTcpSocket_->disconnectFromHost();
}

void DataTransmitter::TcpReadyReadSlot(qintptr socketHandle) {
	QTcpSocket* socket = tcpReceiver_[socketHandle]->socket;
	QByteArray* dataBuf = tcpReceiver_[socketHandle]->dataBuf;
	int dataSize = tcpReceiver_[socketHandle]->dataSize;
	int leftSize = dataSize - dataBuf->size();

	QByteArray tmpBuf = socket->read(bufSize_);
	char flag = tmpBuf[bufSize_ - 1];

	if ('h' == flag) {  // ˵����ͷ��
		tmpBuf.resize(bufSize_ - 1);   // ȥ����־λ
		tcpReceiver_[socketHandle]->dataSize = tmpBuf.toInt();
		dataBuf->clear();
	}
	else if ('c' == flag) { // ˵��������
		if (leftSize >= bufSize_ - 1) tmpBuf.resize(bufSize_ - 1);    // ȥ����־λ
		else tmpBuf.resize(leftSize);    // ȥ����λ�ͱ�־λ
		dataBuf->append(tmpBuf);
	}
	else if ('e' == flag) { // ˵����β��
		assert(dataSize == dataBuf->size());
		socket->disconnectFromHost();
		delete socket;
		tcpReceiver_.erase(socketHandle);
		emit TcpReceive(dataBuf);
	}
}

bool DataTransmitter::IsSelf(QString& ip) {
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	for (auto& i : ipAddressesList) 
		if (ip == i.toString()) return true;
	return false;
}

// ��ӡ��ǰ�豸��IPv4��ַ(-----����------)
void DataTransmitter::PrintIp() {
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	for (auto& i : ipAddressesList)
		if (i != QHostAddress::LocalHost && i.toIPv4Address()) 
			std::cout << i.toString().toStdString() << std::endl;
	/*
	169.254.24.189
	169.254.252.28
	169.254.92.118     // 169.254.�������������ip��ַ,����Ŀ�����
	192.168.42.1
	192.168.72.1
	211.83.106.229*/
}