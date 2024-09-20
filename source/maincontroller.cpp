#include "maincontroller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

MainController::MainController(QObject *parent)
    : QObject(parent), client(new TCPClient(this)), updateTimer(new QTimer(this)) {

  connect(client, &TCPClient::dataReceived, this, &MainController::onClientResponse);
  connect(updateTimer, &QTimer::timeout, this, &MainController::requestBatteryInfo);
}

void MainController::startAutoUpdate(int intervalMs) {
  updateTimer->start(intervalMs);
}

void MainController::stopAutoUpdate() {
  updateTimer->stop();
}

void MainController::requestBatteryInfo() {
  client->sendBatteryInfoRequest();
}

void MainController::handleSleepRequest() {
  client->sendSleepRequest();
}

void MainController::handleHibernateRequest() {
  client->sendHibernateRequest();
}

void MainController::onClientResponse(const QByteArray &response) {
  QJsonDocument doc = QJsonDocument::fromJson(response);
  if (!doc.isObject()) {
    qDebug() << "Invalid response format";
    return;
  }

  QJsonObject obj = doc.object();
  emit batteryInfoReceived(obj);
}
