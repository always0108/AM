#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class Tcpclient : public QTcpSocket
{
    Q_OBJECT
public:
    Tcpclient(QObject *parent = nullptr);
    ~Tcpclient();
    QString IP ="127.0.0.1";
    int Port = 8010;
    bool tcpstatus = false;
    QString msg_send;
    QString msg_recv;
    int msg_flag=0;
    QTcpSocket *socket;

signals:

public slots:
    void on_pushButton_Connect_clicked();
    void on_pushButton_Send_clicked();
    void socket_Read_Data();
    void socket_Disconnected();
    void disconnectedFromHost();
    bool getTcpStatus();
};

#endif // TCPCLIENT_H
