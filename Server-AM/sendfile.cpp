#include "sendfile.h"
#include <QFile>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QApplication>
#include <QtMath>
#include <iostream>

SendFile::SendFile(QDialog *parent) : QDialog(parent)
{
    tPort = 8012;
    tSrv = new QTcpSocket();
    initSrv();
}

SendFile::~SendFile()
{
    delete tSrv;
    delete locFile;
}

void SendFile::initSrv()
{
    payloadSize = 64*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesTobeWrite = 0;
    outBlock.clear();
    tSrv->close();
}

// 发送文件
void SendFile::sndMsg()
{
    initSrv();
    tSrv->abort();
    tSrv->connectToHost(IP, tPort);
    locFile = new QFile(filename);
    if(!locFile->open(QFile::ReadOnly)){
        return;
    }
    totalBytes = locFile->size();
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_12);
    QString curFile = filename.right(filename.size()-filename.lastIndexOf('/')-1);
    // 文件流先填入2个0，第一个0写入总大小，第二个0预留写入此次发送文件的大小
    sendOut << qint64(0) << qint64(0) << curFile;
    totalBytes += outBlock.size();
    sendOut.device()->seek(0);
    sendOut << totalBytes << qint64((outBlock.size()-sizeof (qint64)*2));
    //发送带报头的文件
    bytesTobeWrite = totalBytes - tSrv->write(outBlock);
    outBlock.resize(0);

    //继续传输数据
    while(bytesTobeWrite  > 0){
        outBlock = locFile->read(qMin(bytesTobeWrite, payloadSize));
        bytesTobeWrite -= tSrv->write(outBlock);
        outBlock.resize(0);
    }
    locFile->close();
    tSrv->close();
    tSrv->disconnectFromHost();
    qDebug() << curFile << "send success";
}
