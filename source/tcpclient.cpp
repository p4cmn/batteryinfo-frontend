// tcpclient.cpp
#include "tcpclient.h"
#include <QDebug>

TCPClient::TCPClient(QObject *parent)
    : QObject(parent), socket(new QTcpSocket(this)), reconnectTimer(new QTimer(this)) {

  connect(socket, &QTcpSocket::connected, this, &TCPClient::onConnected);
  connect(socket, &QTcpSocket::disconnected, this, &TCPClient::onDisconnected);
  connect(socket, &QTcpSocket::readyRead, this, &TCPClient::onReadyRead);
  connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &TCPClient::onError);

  reconnectTimer->setInterval(5000);
  connect(reconnectTimer, &QTimer::timeout, this, [this]() {
    tryReconnect(QHostAddress::LocalHost, 9090);
  });
}

void TCPClient::startClient(const QHostAddress &address, quint16 port) {
  attemptConnection(address, port);
}

void TCPClient::stopClient() {
  if (socket->isOpen()) {
    socket->disconnectFromHost();
    reconnectTimer->stop();
    qDebug() << "Client disconnected from server.";
  }
}

void TCPClient::sendBatteryInfoRequest() {
  QJsonObject request;
  request["action"] = "getBatteryInfo";
  sendRequest(request);
}

void TCPClient::sendSleepRequest() {
  QJsonObject request;
  request["action"] = "sleep";
  sendRequest(request);
}

void TCPClient::sendHibernateRequest() {
  QJsonObject request;
  request["action"] = "hibernate";
  sendRequest(request);
}

void TCPClient::tryReconnect(const QHostAddress &address, quint16 port) {
  if (socket->state() == QAbstractSocket::UnconnectedState) {
    attemptConnection(address, port);
  } else {
    qDebug() << "Socket is already trying to connect or connected.";
  }
}

void TCPClient::attemptConnection(const QHostAddress &address, quint16 port) {
  qDebug() << "Connecting to server at" << address.toString() << "on port" << port << "...";
  socket->connectToHost(address, port);

  if (!socket->waitForConnected(3000)) {
    qDebug() << "Failed to connect to server, will retry...";
    reconnectTimer->start();
  } else {
    qDebug() << "Connected to server!";
    reconnectTimer->stop();
  }
}

void TCPClient::sendRequest(const QJsonObject &request) {
  QJsonDocument doc(request);
  QByteArray data = doc.toJson();
  socket->write(data);
  socket->flush();
}

void TCPClient::onConnected() {
  qDebug() << "Successfully connected to the server!";
  reconnectTimer->stop();
}

void TCPClient::onReadyRead() {
  QByteArray response = socket->readAll();
  emit dataReceived(response);
}

void TCPClient::onDisconnected() {
  qDebug() << "Disconnected from server, will try to reconnect...";
  reconnectTimer->start();
}

void TCPClient::onError(QAbstractSocket::SocketError) {
  qDebug() << "Error:" << socket->errorString();
}
