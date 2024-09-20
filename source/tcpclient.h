#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

class TCPClient : public QObject {
  Q_OBJECT

private:
  QTcpSocket *socket;

public:
  explicit TCPClient(QObject *parent = nullptr);

  void connectToServer(const QString &host, quint16 port);
  void sendBatteryInfoRequest();
  void sendSleepRequest();
  void sendHibernateRequest();

private:
  void sendRequest(const QJsonObject &request);

private slots:
  void onConnected();
  void onReadyRead();
  void onDisconnected();
  void onError(QAbstractSocket::SocketError socketError);
};

#endif // TCPCLIENT_H
