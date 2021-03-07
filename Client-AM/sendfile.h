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

    void initSrv();
    QString IP = "127.0.0.1";
    int tPort = 8011;

signals:
    //void sndFileName(QString fileName);


private:
    QLineEdit *fileLine;
    QLabel *filelabel;
    QLabel *statuslabel;
    QPushButton *openfilebtn;
    QPushButton *sendbtn;
    QPushButton *closebtn;
    QProgressBar *prossbar;

    QTcpSocket *tSrv;
    QString filename;
    QString theFileName;
    QFile *locFile;
    qint64 totalBytes;
    qint64 bytesWritten;
    qint64 bytesTobeWrite;
    qint64 payloadSize;
    QByteArray outBlock;
    QTime time;

public slots:
    // 发送文件
    void sndMsg();
    // 传输进度条
    void updClntProgress(qint64 numBytes);
    // 选择要发送的文件
    void on_openBtn_clicked();
    // 发送文件
    void on_sendBtn_clicked();
    // 关闭窗口
    void on_closeBtn_clicked();
};

#endif // SENDFILE_H
