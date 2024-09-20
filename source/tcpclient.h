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

signals:
  void dataReceived(const QByteArray &data);  // Сигнал для передачи данных

public slots:
  void onReadyRead();  // Слот для обработки ответа сервера
  void onConnected();
  void onDisconnected();
  void onError(QAbstractSocket::SocketError socketError);

private:
  void sendRequest(const QJsonObject &request);
};

#endif // TCPCLIENT_H
