#include "tcpclient.h"
#include <iostream>
#include <showwidget.h>
Tcpclient::Tcpclient(QObject *parent) : QTcpSocket(parent)
{
    socket = new QTcpSocket();
    connect(socket, &QTcpSocket::readyRead, this, &Tcpclient::socket_Read_Data);
    connect(socket, &QTcpSocket::disconnected, this, &Tcpclient::disconnectedFromHost);
}

void Tcpclient::on_pushButton_Connect_clicked()
{
      //取消已有的连接
      socket->abort();
      //连接服务器
      socket->connectToHost(IP, Port);
      //等待连接成功
     if(!socket->waitForConnected(200))
     {
         tcpstatus = false;
         return;
     }
     tcpstatus = true;
}

void Tcpclient::on_pushButton_Send_clicked()
{
    socket->write(msg_send.toLatin1());
    socket->flush();
}

void Tcpclient::socket_Read_Data()
{
    msg_flag=0;
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        msg_recv = tr(buffer);
        msg_flag=1;
    }
}

void Tcpclient::socket_Disconnected()
{
    //断开连接
    socket->disconnectFromHost();
    socket->waitForDisconnected();
    tcpstatus = false;
}

void Tcpclient::disconnectedFromHost()
{
    tcpstatus = false;
}

bool Tcpclient::getTcpStatus()
{
    return tcpstatus;
}
