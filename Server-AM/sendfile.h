#ifndef SENDFILE_H
#define SENDFILE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QGridLayout>
#include <QTcpSocket>

class QFile;
class QTcpSocket;

class SendFile : public QWidget
{
    Q_OBJECT
public:
    explicit SendFile(QWidget *parent = nullptr);

    QString IP;
    QString fileName;
    quint16 tPort = 8012;
    void initSrv();
    void sndMsg();

private:
    QTcpSocket *tSrv;
    QString theFileName;
    QFile *locFile;
    qint64 totalBytes;
    qint64 bytesWritten;
    qint64 bytesTobeWrite;
    qint64 payloadSize;
    QByteArray outBlock;
};

#endif // SENDFILE_H
