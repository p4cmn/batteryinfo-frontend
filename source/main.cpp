#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  TCPClient client;
  if (!client.startClient(QHostAddress::LocalHost, 9090)) {
    qDebug() << "Failed to connect to the server. Exiting...";
    return -1;
  }

  MainController controller(&client);

  MainWindow mainWindow(&controller);
  mainWindow.show();
  return a.exec();
}
