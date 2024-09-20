#include "tcpclient.h"
#include <QDebug>

TCPClient::TCPClient(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)) {
  connect(socket, &QTcpSocket::connected, this, &TCPClient::onConnected);
  connect(socket, &QTcpSocket::disconnected, this, &TCPClient::onDisconnected);
  connect(socket, &QTcpSocket::readyRead, this, &TCPClient::onReadyRead);
  connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &TCPClient::onError);
}

bool TCPClient::startClient(const QHostAddress &address, quint16 port) {
  qDebug() << "Connecting to server...";
  socket->connectToHost(address, port);
  if (!socket->waitForConnected(3000)) {
    qDebug() << "Failed to connect to server";
    return false;
  }
  return true;
}

void TCPClient::stopClient() {
  if (socket->isOpen()) {
    socket->disconnectFromHost();
    qDebug() << "Client disconnected from server.";
  }
}

void TCPClient::sendBatteryInfoRequest() {
  QJsonObject request;
  request["action"] = "getBatteryInfo";
  QJsonDocument doc(request);
  QByteArray data = doc.toJson();
  socket->write(data);
  socket->flush();
}

void TCPClient::sendSleepRequest() {
  QJsonObject request;
  request["action"] = "sleep";
  QJsonDocument doc(request);
  QByteArray data = doc.toJson();
  socket->write(data);
  socket->flush();
}

void TCPClient::sendHibernateRequest() {
  QJsonObject request;
  request["action"] = "hibernate";
  QJsonDocument doc(request);
  QByteArray data = doc.toJson();
  socket->write(data);
  socket->flush();
}

void TCPClient::onConnected() {
  qDebug() << "Successfully connected to the server!";
}

void TCPClient::onReadyRead() {
  QByteArray response = socket->readAll();
  emit dataReceived(response);
}

void TCPClient::onDisconnected() {
  qDebug() << "Disconnected from server.";
}

void TCPClient::onError(QAbstractSocket::SocketError) {
  qDebug() << "Error:" << socket->errorString();
}
