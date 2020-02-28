#include "data_transmitter.h"
#include <assert.h>

DataTransmitter::DataTransmitter(QObject* parent):QObject(parent){
	// 获取Ip--返回ip列表的最后一个（暂时这么干）
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
		// 需要换成日志打印调试信息
		std::cout << "TcpServer启动，监听端口失败！\n";
		tcpServer_->close();
	}
}

void DataTransmitter::incomingConnection(qintptr socketHandle) {
	QTcpSocket* newSocket = new QTcpSocket();
	newSocket->setSocketDescriptor(socketHandle);
	// 超出最大连接数量
	if (tcpReceiver_.size() > tcpServer_->maxPendingConnections()) {
		newSocket->disconnectFromHost();
		delete newSocket;
		return;
	}
	// 否则记录下来
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
	// 自己发来的就跳过
	if(!IsSelf(*senderIp)) emit UdpReceive(data, senderIp);
}

void DataTransmitter::TcpSendP2P(const QByteArray &data, const QString &receiverIp) {
	// 注意，所有块小均为bufSize_,每块最后一字节是标志位（为了效率）
	// 先连接
	int tryTimes = 0;
TRY_CONNECT:
	sendTcpSocket_->connectToHost(QHostAddress(receiverIp), recvTcpPort_);
CONNECT_OK:
	if (sendTcpSocket_->state() == QTcpSocket::UnconnectedState) {
		if (++tryTimes == 3) {
			// 记录日志
			return;
		}
		Sleep(1000);
		goto TRY_CONNECT;
	}

	// 然后发头部:size
	int size = data.size();
	QByteArray head = QByteArray::number(size);
	head.resize(bufSize_);
	head[bufSize_ - 1] = 'h';  // 代表这是头部
	if (bufSize_ != sendTcpSocket_->write(head)) {
		// 记录日志
		goto CONNECT_OK;
	}; 

	// 然后发内容
	int leftSize = size;
	const char* p = data;
	while (leftSize >= bufSize_ - 1) {
		QByteArray content(p, bufSize_ - 1);
		content[bufSize_ - 1] = 'c'; // 代表这是内容
		leftSize -= (bufSize_ - 1);
		p += (bufSize_ - 1);
		if (bufSize_ != sendTcpSocket_->write(content)) {
			// 记录日志
			goto CONNECT_OK;
		};
	}

	// 到这里可能leftSize == 0、leftSize > 0
	if (bufSize_ > 0) {
		QByteArray content(p, leftSize);
		content.resize(bufSize_);
		content[bufSize_ - 1] = 'c';
		if (bufSize_ != sendTcpSocket_->write(content)) {
			// 记录日志
			goto CONNECT_OK;
		};
	}

	// 到这里表示内容发送完成，再发一个结束标志
	QByteArray content(bufSize_, '\0');
	content[bufSize_ - 1] = 'e';
	if (bufSize_ != sendTcpSocket_->write(content)) {
		// 记录日志
		goto CONNECT_OK;
	};

	// 完成此次传输，断开连接
	sendTcpSocket_->disconnectFromHost();
}

void DataTransmitter::TcpReadyReadSlot(qintptr socketHandle) {
	QTcpSocket* socket = tcpReceiver_[socketHandle]->socket;
	QByteArray* dataBuf = tcpReceiver_[socketHandle]->dataBuf;
	int dataSize = tcpReceiver_[socketHandle]->dataSize;
	int leftSize = dataSize - dataBuf->size();

	QByteArray tmpBuf = socket->read(bufSize_);
	char flag = tmpBuf[bufSize_ - 1];

	if ('h' == flag) {  // 说明是头部
		tmpBuf.resize(bufSize_ - 1);   // 去除标志位
		tcpReceiver_[socketHandle]->dataSize = tmpBuf.toInt();
		dataBuf->clear();
	}
	else if ('c' == flag) { // 说明是内容
		if (leftSize >= bufSize_ - 1) tmpBuf.resize(bufSize_ - 1);    // 去除标志位
		else tmpBuf.resize(leftSize);    // 去除空位和标志位
		dataBuf->append(tmpBuf);
	}
	else if ('e' == flag) { // 说明是尾部
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

// 打印当前设备的IPv4地址(-----乱入------)
void DataTransmitter::PrintIp() {
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	for (auto& i : ipAddressesList)
		if (i != QHostAddress::LocalHost && i.toIPv4Address()) 
			std::cout << i.toString().toStdString() << std::endl;
	/*
	169.254.24.189
	169.254.252.28
	169.254.92.118     // 169.254.是虚拟机的网络ip地址,下面的可以用
	192.168.42.1
	192.168.72.1
	211.83.106.229*/
}