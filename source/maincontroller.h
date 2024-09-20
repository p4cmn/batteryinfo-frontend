#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QTimer>
#include "tcpclient.h"

class MainController : public QObject {
  Q_OBJECT

private:
  TCPClient* client;
  QTimer* updateTimer;

public:
  explicit MainController(TCPClient* client, QObject *parent = nullptr);
  void startAutoUpdate(int intervalMs);
  void stopAutoUpdate();
  void requestBatteryInfo();
  void handleSleepRequest();
  void handleHibernateRequest();

signals:
  void batteryInfoReceived(const QJsonObject &info);

private slots:
  void onClientResponse(const QByteArray &response);

};

#endif // MAINCONTROLLER_H
