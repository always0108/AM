#include "recvfile.h"
#include <QDebug>
#include <QMessageBox>
#include <QDataStream>

Recvfile::Recvfile(QObject *parent) : QTcpServer(parent)
{
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
    tPort = 8012;
    Revserver = new QTcpServer();
    tClnt = new QTcpSocket();
    connect(Revserver,&QTcpServer::newConnection,this,&Recvfile::newConn);
}

Recvfile::~Recvfile()
{
    delete Revserver;
    delete tClnt;
    delete locFile;
}

void Recvfile::Listen_action()
{
    if(!Revserver->listen(QHostAddress::Any, tPort))
    {
        qDebug()<<Revserver->errorString();
        return;
    }
}

// 建立新的Socket连接
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

// 读取文件并保存到本地
void Recvfile::readMsg()
{
    QDataStream in(tClnt);
    in.setVersion(QDataStream::Qt_5_12);
    if(bytesReceived <= sizeof(qint64)*2){
        if((tClnt->bytesAvailable()>=fileNameSize) && (fileNameSize==0)){
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64)*2;
        }
        if((tClnt->bytesAvailable() >= fileNameSize) && (fileNameSize != 0)){
            in >> fileName;
            //本地测试
            filePath = "D:\\layers\\"+fileName;
            locFile = new QFile(filePath);
            bytesReceived += fileNameSize;
            if(!locFile->open(QFile::WriteOnly)){
                qDebug() << "open fail";
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
        showWidget->log("获取云端文件完成");
        showWidget->showpath->clearpath();
        showWidget->showpath->plotpath(filePath);
        showWidget->log("路径已展示");
    }
}

void Recvfile::setShowWidget(ShowWidget *showWidget)
{
    this->showWidget = showWidget;
}
