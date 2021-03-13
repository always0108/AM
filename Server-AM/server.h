#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QProcess>
#include <sys/stat.h>
#include "sendfile.h"
#include "recvfile.h"
#include "sqlAction.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();
    void init();
    void Listen_action();
    void Perform_action(); 

private slots:
    void Messageclassify();
    void Send_action();
    void server_New_Connect();
    void socket_Read_Data();
    void socket_Disconnected();
    void on_readoutput();
    void on_readerror();

private:
    int port;
    QTcpServer* server;
    QTcpSocket* socket;
    SqlAction *sqlAction;
    Recvfile *server_file;
    SendFile *sendfile;
    QString path;
    int flag;
    QString temp;
    QString signal_recv;
    QString msg_recv;
    QString msg_send;
    QProcess* cmd;
    QString fileName;
    QTcpSocket *client;
    QHostAddress clientip;
    QString parallelStyle;
    QString infillStyle;
    int layer_nr = 0;
    double thickness = 0;
    //数据库后端分页的大小
    int pageSize;
    int currentPage;
    //打印参数的设置
    QString printSettings;
    int laserPower = 110;
    int scanSpeed = 200;
    int layerThickness = 50;
    int scanPitch = 50;
    int spotSize = 60;
};

//使用stat函数获取文件状态，成功则存在，否则不存在,比较几种方式，用stat() 函数的性能最好
inline bool fileExist(const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}
#endif // SERVER_H
