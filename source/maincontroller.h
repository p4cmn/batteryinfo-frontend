#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include "tcpclient.h"

class MainController : public QObject {
  Q_OBJECT

private:
  TCPClient* client;

public:
  explicit MainController(QObject *parent = nullptr);

  void requestBatteryInfo();
  void handleSleepRequest();
  void handleHibernateRequest();

signals:
  void batteryInfoReceived(const QJsonObject &info);

private slots:
  void onClientResponse(const QByteArray &response);

};

#endif // MAINCONTROLLER_H
