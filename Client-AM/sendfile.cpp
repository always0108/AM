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
    setWindowTitle("发送文件");
    setFixedSize(300,200);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    fileLine = new QLineEdit();
    fileLine->setFixedSize(140,25);
    fileLine->setFocusPolicy(Qt::NoFocus);
    filelabel = new QLabel(" 选择文件");
    filelabel->setFixedSize(60,25);
    statuslabel = new QLabel();
    statuslabel->setFixedWidth(200);
    openfilebtn = new QPushButton("打开");
    openfilebtn->setFixedSize(50,25);
    openfilebtn->setFocusPolicy(Qt::NoFocus);
    sendbtn = new QPushButton("发送");
    sendbtn->setFixedSize(50,25);
    sendbtn->setFocusPolicy(Qt::NoFocus);
    closebtn = new QPushButton("关闭");
    closebtn->setFixedSize(50,25);
    closebtn->setFocusPolicy(Qt::NoFocus);
    prossbar = new QProgressBar();
    prossbar->setFixedSize(220,25);
    QGridLayout *mainlayout = new QGridLayout(this);
    mainlayout->setMargin(5);
    mainlayout->setSpacing(5);
    mainlayout->addWidget(filelabel,0,0);
    mainlayout->addWidget(fileLine,0,1);
    mainlayout->addWidget(openfilebtn,0,2);
    mainlayout->addWidget(sendbtn,1,0);
    mainlayout->addWidget(prossbar,1,1,1,2);
    mainlayout->addWidget(statuslabel,2,0,1,2);
    mainlayout->addWidget(closebtn,3,2);

    tPort = 8011;
    tSrv = new QTcpSocket();
    connect(tSrv, &QTcpSocket::bytesWritten, this, [=](){updClntProgress(payloadSize);});
    connect(openfilebtn, &QPushButton::pressed, this, &SendFile::on_openBtn_clicked);
    connect(sendbtn, &QPushButton::pressed, this, &SendFile::on_sendBtn_clicked);
    connect(closebtn, &QPushButton::pressed, this, &QDialog::close);
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
    statuslabel->setText("请选择要传送的文件");
    prossbar->reset();
    openfilebtn->setEnabled(true);
    sendbtn->setEnabled(false);
    tSrv->close();
}

// 发送文件
void SendFile::sndMsg()
{
    sendbtn->setEnabled(true);
    statuslabel->setText("开始传送文件" + theFileName+" !");
    locFile = new QFile(filename);
    if(!locFile->open(QFile::ReadOnly)){
        QMessageBox::warning(this,tr("应用程序"),tr("无法读取文件 %l:\n%2").arg(filename).arg(locFile->errorString()));
        return;
    }
    totalBytes = locFile->size();
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);

    sendOut.setVersion(QDataStream::Qt_5_12);
    time.start();
    QString curFile = filename.right(filename.size()-filename.lastIndexOf('/')-1);
    // 文件流先填入2个0，第一个0写入总大小，第二个0预留写入此次发送文件的大小
    sendOut <<qint64(0) << qint64(0) << curFile;
    totalBytes += outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64((outBlock.size()-sizeof (qint64)*2));

    bytesTobeWrite = totalBytes - tSrv->write(outBlock);
    bytesWritten += outBlock.size();
    outBlock.resize(0);
}

// 传输进度条
void SendFile::updClntProgress(qint64 numBytes)
{
    bytesWritten += qMin(bytesTobeWrite, numBytes);
    if(bytesTobeWrite > 0){
        outBlock = locFile->read(qMin(bytesTobeWrite, payloadSize));
        bytesTobeWrite -= (int)tSrv->write(outBlock);
        outBlock.resize(0);
    }else{
        locFile->close();
    }
    prossbar->setMaximum(totalBytes);
    prossbar->setValue(bytesWritten);
    float useTime = time.elapsed();
    double speed = bytesWritten/useTime;
    statuslabel->setText("已发送"+QString::number(bytesWritten/(1024*1024))+"MB "+
                         "("+QString::number(speed*1000/(1024*1024))+"MB/s) \n "+
                         "共"+QString::number(totalBytes/(1024*1024))+"MB 已用时:"+
                         QString::number(useTime/1000)+" 秒\n"+
                         "估计剩余时间:"+QString::number((totalBytes/speed/1000)-useTime/1000)+" 秒");
    if(bytesWritten==totalBytes){
        locFile->close();
        tSrv->close();
        statuslabel->setText("传输文件"+theFileName+"成功!");
        openfilebtn->setEnabled(true);
        sendbtn->setEnabled(false);
        tSrv->disconnectFromHost();
    }
}

// 选择要发送的文件
void SendFile::on_openBtn_clicked()
{
    filename = QFileDialog::getOpenFileName(this);
    fileLine->setText(filename);
    if(!filename.isEmpty()){
        theFileName = filename.right(filename.size() - filename.lastIndexOf('/') -1);
        statuslabel->setText("要传输的文件为:" + theFileName);
        sendbtn->setEnabled(true);
        openfilebtn->setEnabled(false);
    }
}

// 发送文件
void SendFile::on_sendBtn_clicked()
{
    initSrv();
    tSrv->abort();
    tSrv->connectToHost(IP, tPort);
    sndMsg();
}

// 关闭窗口
void SendFile::on_closeBtn_clicked()
{
    tSrv->close();
    if(locFile->isOpen()){
       locFile->close();
    }
    tSrv->deleteLater();
}
