#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QTime>
#include "batteryhealth.h"
#include "batterytype.h"
#include "powermode.h"
#include "powersupplytype.h"
#include "maincontroller.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
  Q_OBJECT

private:
  Ui::MainWindow* ui;
  QSystemTrayIcon* trayIcon;
  QMenu* trayMenu;
  MainController* controller;

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void handleSleepButtonClicked();
  void handleHibernateButtonClicked();
  void onBatteryInfoReceived(const QJsonObject &info);
  void quit();
  void toggleVisibility();

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  void setupSystemTray();

public:
  void updateChargeLevelProgressBar(int chargeLevel);
  void updatePowerMode(PowerMode powerMode);
  void updateRemainingBatteryLifetime(const QTime& lifetime);
  void updatePowerSupplyType(PowerSupplyType supplyType);
  void updateDischargeTime(const QTime& dischargeTime);
  void updateBatteryType(BatteryType type);
  void updateDesignMaxCapacity(double capacity);
  void updateCurrentMaxCapacity(double capacity);
  void updateVoltage(double voltage);
  void updateBatteryHealth(BatteryHealth health);
};

#endif // MAINWINDOW_H
