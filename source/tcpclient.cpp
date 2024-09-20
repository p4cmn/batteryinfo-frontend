#include "tcpclient.h"
#include <QDebug>

TCPClient::TCPClient(QObject *parent): QObject(parent) {
  connect(socket, &QTcpSocket::connected, this, &TCPClient::onConnected);
  connect(socket, &QTcpSocket::disconnected, this, &TCPClient::onDisconnected);
  connect(socket, &QTcpSocket::readyRead, this, &TCPClient::onReadyRead);
  connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &TCPClient::onError);
}

void TCPClient::connectToServer(const QString &host, quint16 port) {
  qDebug() << "Connecting to server...";
  socket->connectToHost(host, port);
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

void TCPClient::onConnected() {
  qDebug() << "Successfully connected to the server!";
}

void TCPClient::onReadyRead() {
  QByteArray response = socket->readAll();
  qDebug() << "Response from server:" << response;
}

void TCPClient::onDisconnected() {
  qDebug() << "Disconnected from server.";
}

void TCPClient::onError(QAbstractSocket::SocketError socketError) {
  qDebug() << "Error:" << socket->errorString();
}

void TCPClient::sendRequest(const QJsonObject &request) {
  QJsonDocument doc(request);
  QByteArray data = doc.toJson();
  socket->write(data);
  socket->flush();
}
