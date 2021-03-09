#include <QCoreApplication>
#include <QApplication>
#include "server.h"
#include "recvfile.h"
#include "SqlAction.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*Server *server = new Server();
    server->Listen_action();
    Recvfile *server_file = new Recvfile();
    server_file->Listen_action();
    */

    //数据库测试

    SqlAction *sqlAction = new SqlAction();
    if(sqlAction->init()){
        //sqlAction->insert("model0.stl");
        sqlAction->getFilesList();
    }
    return a.exec();
}
