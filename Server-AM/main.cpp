#include <QCoreApplication>
#include <QApplication>
#include "server.h"
#include "recvfile.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server *server = new Server();
    server->Listen_action();
    Recvfile *server_file = new Recvfile();
    server_file->Listen_action();

    return a.exec();
}
