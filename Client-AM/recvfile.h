#ifndef RECVFILE_H
#define RECVFILE_H

#include <QObject>
#include <QHostAddress>
#include <QFile>
#include <QTime>
#include <QCloseEvent>
#include <QTcpSocket>
#include <QTcpServer>
#include "showwidget.h"

class Recvfile : public QTcpServer
{
    Q_OBJECT
public:
    explicit Recvfile(QObject *parent = nullptr);
    ~Recvfile();
    void setFileName(QString name);
    void setShowWidget(ShowWidget *showWidget);
    void Listen_action();
private:
    QTcpServer *Revserver;
    QTcpSocket *tClnt;
    quint16 blockSize;
    QHostAddress hostAddr;
    qint16 tPort;
    qint64 totalBytes;
    qint64 bytesReceived;
    qint64 fileNameSize;
    QString fileName;
    QFile *locFile;
    QByteArray inBlock;
    QTime time;
    QString status;
    QString filePath;
    //状态输出栏
    ShowWidget *showWidget;

private slots:
    // 建立Socket连接
    void newConn();
    // 读取消息
    void readMsg();
};

#endif // RECVFILE_H
