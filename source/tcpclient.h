#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

class TCPClient : public QObject {
  Q_OBJECT

private:
  QTcpSocket *socket;
  QTimer *reconnectTimer;

public:
  explicit TCPClient(QObject *parent = nullptr);

  void startClient(const QHostAddress &address, quint16 port);
  void stopClient();

  void sendBatteryInfoRequest();
  void sendSleepRequest();
  void sendHibernateRequest();

signals:
  void dataReceived(const QByteArray &data);

private:
  void attemptConnection(const QHostAddress &address, quint16 port);
  void sendRequest(const QJsonObject &request);

public slots:
  void tryReconnect(const QHostAddress &address, quint16 port);

private slots:
  void onConnected();
  void onReadyRead();
  void onDisconnected();
  void onError(QAbstractSocket::SocketError socketError);
};

#endif // TCPCLIENT_H
