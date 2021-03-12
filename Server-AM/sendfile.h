#ifndef SENDFILE_H
#define SENDFILE_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QGridLayout>
#include <QTime>

class QFile;
class QTcpSocket;

class SendFile : public QDialog
{
    Q_OBJECT
public:
    explicit SendFile(QDialog *parent = nullptr);
    ~SendFile();
    void initSrv();
    QString IP;
    int tPort;
    QString filename;

private:
    QTcpSocket *tSrv;
    QString theFileName;
    QFile *locFile;
    qint64 totalBytes;
    qint64 bytesWritten;
    qint64 bytesTobeWrite;
    qint64 payloadSize;
    QByteArray outBlock;

public slots:
    // 发送文件
    void sndMsg();
};

#endif // SENDFILE_H
