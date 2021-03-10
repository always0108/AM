#ifndef RECVFILE_H
#define RECVFILE_H

#include <QObject>
#include <QHostAddress>
#include <QFile>
#include <QTime>
#include <QCloseEvent>
#include <QTcpSocket>
#include <QTcpServer>
#include "sqlAction.h"

class Recvfile : public QTcpServer
{
    Q_OBJECT
public:
    explicit Recvfile(QObject *parent = nullptr);
    ~Recvfile();
    void setFileName(QString name);
    void setDatabase(SqlAction *sqlAction);
    void Listen_action();

private:
    QTcpServer *Revserver;
    QTcpSocket *tClnt;
    quint16 blockSize;
    QHostAddress hostAddr;
    quint16 tPort;
    qint64 totalBytes;
    qint64 bytesReceived;
    qint64 fileNameSize;
    QString fileName;
    QFile *locFile;
    QByteArray inBlock;
    QTime time;
    SqlAction *sqlAction;

private slots:
    void newConn();
    void readMsg();

signals:

};

#endif // RECVFILE_H
