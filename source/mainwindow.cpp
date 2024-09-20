#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    trayIcon(nullptr),
    trayMenu(nullptr),
    controller(new MainController(this)) {

  ui->setupUi(this);

  connect(controller, &MainController::batteryInfoReceived, this, &MainWindow::onBatteryInfoReceived);

  connect(ui->sleepButton, &QPushButton::clicked, this, &MainWindow::handleSleepButtonClicked);
  connect(ui->hibernateButton, &QPushButton::clicked, this, &MainWindow::handleHibernateButtonClicked);
  controller->startAutoUpdate(1000);

  setupSystemTray();
}

MainWindow::~MainWindow() {
  delete ui;
  controller->stopAutoUpdate();
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (trayIcon) {
    hide();
    event->ignore();
  }
}

void MainWindow::setupSystemTray() {
  QIcon icon(":/icons/battery.ico");
  if (icon.isNull()) {
    qWarning() << "Icon not loaded. Check the path.";
    return;
  }

  trayIcon = new QSystemTrayIcon(icon, this);
  trayIcon->setToolTip("Battery Information");

  trayMenu = new QMenu(this);
  QAction *showAction = trayMenu->addAction("Show");
  QAction *quitAction = trayMenu->addAction("Quit");

  trayIcon->setContextMenu(trayMenu);

  connect(trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
      toggleVisibility();
    }
  });

  connect(showAction, &QAction::triggered, this, [this]() {
    toggleVisibility();
  });

  connect(quitAction, &QAction::triggered, this, &MainWindow::quit);

  trayIcon->show();
}

void MainWindow::toggleVisibility() {
  if (isVisible()) {
    hide();
  } else {
    show();
    raise();
    activateWindow();
  }
}

void MainWindow::quit() {
  qApp->quit();
}

void MainWindow::onBatteryInfoReceived(const QJsonObject &info) {
  updateChargeLevelProgressBar(info["chargeLevel"].toInt());
  updatePowerMode(static_cast<PowerMode>(info["powerMode"].toInt()));
  updateRemainingBatteryLifetime(QTime::fromString(info["remainingBatteryLifetime"].toString(), "hh:mm:ss"));
  updatePowerSupplyType(static_cast<PowerSupplyType>(info["powerSupplyType"].toInt()));
  updateDischargeTime(QTime::fromString(info["dischargeTime"].toString(), "hh:mm:ss"));
  updateBatteryType(static_cast<BatteryType>(info["batteryType"].toInt()));
  updateDesignMaxCapacity(info["maxCapacity"].toDouble());
  updateCurrentMaxCapacity(info["currentCapacity"].toDouble());
  updateVoltage(info["voltage"].toDouble());
  updateBatteryHealth(static_cast<BatteryHealth>(info["batteryHealth"].toInt()));
}

void MainWindow::handleSleepButtonClicked() {
  controller->handleSleepRequest();
}

void MainWindow::handleHibernateButtonClicked() {
  controller->handleHibernateRequest();
}

void MainWindow::updateChargeLevelProgressBar(int chargeLevel) {
  ui->chargeLevelProgressBar->setValue(chargeLevel);
}

void MainWindow::updatePowerMode(PowerMode powerMode) {
  QString mode;
  switch(powerMode) {
    case PowerMode::Ordinary:
      mode = "Ordinary";
      break;
    case PowerMode::Saving:
      mode = "Saving";
      break;
    case PowerMode::Undefined:
      mode = "Undefined";
      break;
  }
  ui->powerModelLabel->setText(mode);
}

void MainWindow::updateRemainingBatteryLifetime(const QTime& lifetime) {
  QString time = lifetime.isValid() ? lifetime.toString("hh:mm:ss") : "Undefined";
  ui->remainingBatteryLifetimeLabel->setText(time);
}

void MainWindow::updatePowerSupplyType(PowerSupplyType supplyType) {
  QString type;
  switch(supplyType) {
    case PowerSupplyType::AlternatingCurrent:
      type = "AC";
      break;
    case PowerSupplyType::DirectCurrent:
      type = "DC";
      break;
    case PowerSupplyType::Unknown:
      type = "UNKNOWN";
      break;
    case PowerSupplyType::Undefined:
      type = "UNDEFINED";
      break;
  }
  ui->powerSupplyTypeLabel->setText(type);
}

void MainWindow::updateDischargeTime(const QTime& dischargeTime) {
  ui->timeAfterPowerOffLabel->setText(dischargeTime.toString("hh:mm:ss"));
}

void MainWindow::updateBatteryType(BatteryType type) {
  QString batType;
  switch(type) {
    case BatteryType::LeadAcid:
      batType = "Lead-Acid";
      break;
    case BatteryType::LithiumIon:
      batType = "Lithium-Ion";
      break;
    case BatteryType::NickelCadmium:
      batType = "Nickel-Cadmium";
      break;
    case BatteryType::NickelMetalHydride:
      batType = "Nickel-Metal-Hydride";
      break;
    case BatteryType::NickelZinc :
      batType = "Nickel-Zinc";
      break;
    case BatteryType::RechargeableAlkalineManganese :
      batType = "Rechargeable Alkaline-Manganese";
      break;
    case BatteryType::Undefined:
      batType = "Undefined";
      break;
  }
  ui->batteryTypeLabel->setText(batType);
}

void MainWindow::updateDesignMaxCapacity(double capacity) {
  QString text = QString::number(capacity) + " (MWh)";
  ui->maxBatteryCapLabel->setText(text);
}

void MainWindow::updateCurrentMaxCapacity(double capacity) {
  QString text = QString::number(capacity) + " (MWh)";
  ui->curBatteryCapLabel->setText(text);
}

void MainWindow::updateVoltage(double voltage) {
  QString text = QString::number(voltage) + " (mV)";
  ui->voltageLabel->setText(text);
}

void MainWindow::updateBatteryHealth(BatteryHealth health) {
  QString status;
  switch(health) {
    case BatteryHealth::Charging:
      status = "Charging";
      break;
    case BatteryHealth::Critical:
      status = "Critical";
      break;
    case BatteryHealth::Discharging:
      status = "Discharging";
      break;
    case BatteryHealth::PowerOnLine:
      status = "Power on line";
      break;
    case BatteryHealth::Undefined:
      status = "Undefined";
      break;
  }
  ui->batteryHealthLabel->setText(status);
}
