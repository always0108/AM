#ifndef RECVFILE_H
#define RECVFILE_H

#include <QObject>
#include <QHostAddress>
#include <QFile>
#include <QTime>
#include <QCloseEvent>
#include <QTcpSocket>
#include <QTcpServer>

class Recvfile : public QTcpServer
{
    Q_OBJECT
public:
    explicit Recvfile(QObject *parent = nullptr);
    void setFileName(QString name);
    void Listen_action();
protected:
    //void closeEvent(QCloseEvent *event);
public:
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
private slots:
    // 建立Socket连接
    void newConn();
    // 读取消息
    void readMsg();
    //void displayErr(QAbstractSocket::SocketError);

signals:

};

#endif // RECVFILE_H
