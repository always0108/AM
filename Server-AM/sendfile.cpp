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

SendFile::SendFile(QWidget *parent) : QWidget(parent)
{
    tPort = 8012;
    tSrv = new QTcpSocket();
    initSrv();
}

void SendFile::initSrv()
{
    payloadSize = 64*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesTobeWrite = 0;
    tSrv->close();
}

void SendFile::sndMsg()
{
    initSrv();
    tSrv->abort();
    tSrv->connectToHost(IP, tPort);
    qDebug() << fileName;
    locFile = new QFile(fileName);
    if(!locFile->open(QFile::ReadOnly)){
        return;
    }
    totalBytes = locFile->size();
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_12);

    //本地测试
    //QString curFile = fileName.right(fileName.size()-fileName.lastIndexOf("\\")-1);

    QString curFile = fileName.right(fileName.size()-fileName.lastIndexOf('/')-1);
    sendOut <<qint64(0) << qint64(0) << curFile;
    totalBytes += outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64((outBlock.size()-sizeof (qint64)*2));

    bytesTobeWrite = totalBytes - tSrv->write(outBlock);
    bytesWritten += outBlock.size();
    outBlock.resize(0);
}
