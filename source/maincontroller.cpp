#include "maincontroller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

MainController::MainController(QObject *parent)
    : QObject(parent), client(new TCPClient(this)) {

  connect(client, &TCPClient::onReadyRead, this, &MainController::onClientResponse);
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
