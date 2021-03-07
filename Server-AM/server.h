#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QProcess>
#include "sendfile.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QWidget *parent = nullptr);

    void Listen_action();

    void Perform_action(); 

private slots:
    void Messageclassify();

    void Send_action();

    void server_New_Connect();

    void socket_Read_Data();

    void socket_Disconnected();

    void on_readoutput();

    void on_readerror();

private:
    int port;
    QTcpServer* server;
    QTcpSocket* socket;

    QString path;

    int flag;
    QString temp;
    QString signal_recv;
    QString msg_recv;
    QString msg_send;
    QProcess* cmd;
    QString fileName;
    QTcpSocket *client;
    QHostAddress clientip;
    QString printSettings;
    QString parallelStyle = "cpu";

    int layer_nr = 0;
    double thickness = 0;

    SendFile *sendfile;

};

#endif // SERVER_H
