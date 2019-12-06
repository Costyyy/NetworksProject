#include "mainwindow.h"
#include "serverconnection.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    serverConnection *conn = new serverConnection;
    const char* ip = "127.0.0.1";
    conn->connectTo(ip);
    MainWindow w(nullptr, conn);
    w.show();
    return a.exec();
}
