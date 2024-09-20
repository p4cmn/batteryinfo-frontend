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

  bool startClient(const QHostAddress &address, quint16 port);
  void stopClient();

  void sendBatteryInfoRequest();
  void sendSleepRequest();
  void sendHibernateRequest();

signals:
  void dataReceived(const QByteArray &data);

public slots:
  void onReadyRead();
  void onConnected();
  void onDisconnected();
  void onError(QAbstractSocket::SocketError socketError);
};

#endif // TCPCLIENT_H
