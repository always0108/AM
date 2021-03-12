#include "recvfile.h"
#include <QDebug>
#include <QMessageBox>
#include <QDataStream>

Recvfile::Recvfile(QObject *parent) : QTcpServer(parent)
{
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
    tPort = 8011;
    Revserver = new QTcpServer();
    tClnt = new QTcpSocket();
    connect(Revserver,&QTcpServer::newConnection,this,&Recvfile::newConn);
}

Recvfile::~Recvfile()
{
    delete Revserver;
    delete tClnt;
}

void Recvfile::Listen_action()
{
    if(!Revserver->listen(QHostAddress::Any, tPort))
    {
        qDebug()<<Revserver->errorString();
        return;
    }
    qDebug()<< "File socket listen succeessfully!";
}

void Recvfile::newConn()
{
    blockSize = 0;
    fileNameSize = 0;
    fileName.clear();
    totalBytes = 0;
    bytesReceived = 0;
    inBlock.clear();
    tClnt = Revserver->nextPendingConnection();
    //连接QTcpSocket的信号槽，以读取新数据
    connect(tClnt, &QTcpSocket::readyRead, this, &Recvfile::readMsg);
}

void Recvfile::readMsg()
{
    QDataStream in(tClnt);
    in.setVersion(QDataStream::Qt_5_12);
    if(bytesReceived <= sizeof(qint64)*2){
        if((tClnt->bytesAvailable() >= fileNameSize) && (fileNameSize == 0)){
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64)*2;
        }
        if((tClnt->bytesAvailable() >= fileNameSize) && (fileNameSize != 0)){
            in >> fileName;
            //本地测试
            QString filePath = "C:\\test\\server\\model\\"+fileName;
            locFile = new QFile(filePath);
            bytesReceived += fileNameSize;
            if(!locFile->open(QFile::WriteOnly)){
                return;
            }
        }else{
            return;
        }
    }
    if(bytesReceived < totalBytes){
        bytesReceived += tClnt->bytesAvailable();
        inBlock = tClnt->readAll();
        locFile->write(inBlock);
        inBlock.resize(0);
    }
    if(bytesReceived == totalBytes){
        locFile->close();
        tClnt->close();
        sqlAction->insert(fileName);
    }
}

void Recvfile::setDatabase(SqlAction *sqlAction)
{
    this->sqlAction = sqlAction;
}
