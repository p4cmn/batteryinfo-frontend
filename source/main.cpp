#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  TCPClient client;
  client.startClient(QHostAddress::LocalHost, 9090);

  MainController controller(&client);

  MainWindow mainWindow(&controller);
  mainWindow.setVisible(false);

  return a.exec();
}
