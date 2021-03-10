#include <QCoreApplication>
#include <QApplication>
#include "server.h"
#include "recvfile.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server *server = new Server();
    server->init();
    return a.exec();
}
