#include "server.h"
#include <QProcess>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

Server::Server(QWidget *parent) : QTcpServer(parent)
{

}

Server::~Server()
{
    delete server;
    delete cmd;
    delete sendfile;
    delete sqlAction;
    delete server_file;
}

void Server::init()
{
    port = 8010;
    parallelStyle = "CPU";  // 默认为CPU加速
    infillStyle = "Hatch";  // 默认填充方式为Hatch
    server = new QTcpServer();
    cmd = new QProcess();
    Listen_action();
    sendfile = new SendFile();
    sqlAction = new SqlAction();
    sqlAction->init();
    server_file = new Recvfile();
    server_file->Listen_action();
    server_file->setDatabase(sqlAction);
    qDebug()<< "Server init work is already!";
    connect(server,&QTcpServer::newConnection,this,&Server::server_New_Connect);
    connect(cmd,&QProcess::readyReadStandardOutput, this, &Server::on_readoutput);
    connect(cmd,&QProcess::readyReadStandardError, this, &Server::on_readerror);
}

void Server::Listen_action()
{
    if(!server->listen(QHostAddress::Any, port)){
        //若出错，则输出错误信息
        qDebug()<<server->errorString();
        return;
    }
    qDebug()<< "Service socket listen succeessfully!";
}

void Server::Messageclassify()
{
    QStringList list = msg_recv.split("/");
    signal_recv =list[0];
    if(list[0]=="previewPath"){
        layer_nr = list[1].toInt();
    }else if(list[0]=="slicing"){
        thickness = list[1].toDouble();
    }else if(list[0]=="printSettings"){
        printSettings = list[1];
    }else if(list[0]=="paths"){
        layer_nr = list[1].toInt();
    }else if(list[0] == "parallel"){
        parallelStyle = list[1];
    }else if(list[0] == "infillStyle"){
        infillStyle = list[1];
    }else if(list[0] == "targetFile"){
        fileName = list[1];
    }else if(list[0] == "getFileList"){
        currentPage = list[1].toInt();
    }else if(list[0] == "setPageSize"){
        pageSize = list[1].toInt();
    }
    Perform_action();
}

void Server::Perform_action()
{
    if(signal_recv == "slicing"){
        /*QString args;
        args = QString::number(layer_nr);
        QStringList arguments;
        arguments <<"-c"<<"sh slicing.sh";
        cmd->start("bash",arguments);
        if(!cmd->waitForStarted()){
            qDebug() <<"failed";
        }
        cmd->waitForFinished();
        if(cmd){
              cmd->close();
        }*/

        //本地测试
        //希望返回切片的层数
        //我这里发送的是假数据，将30改成每次切片的真正层数即可
        flag = 1;
        msg_send = "slice/30";
    }else if(signal_recv == "infill"){
        /*QStringList arguments;
        arguments <<"-c"<<"/home/zero/function/infill/build/infill -l 100 -s sparseInfillLineDistance=200";
        cmd->start("bash",arguments);
        if(!cmd->waitForStarted()){
            qDebug() <<"failed";
        }
        cmd->waitForFinished();
        if(cmd){
              cmd->close();
        }*/
        //本地测试
        //程序要设置为无缓冲输出，返回目前填充的层数即可
        //output为我在本地测试模拟路径填充的结果，输出1-30，步长为1，每秒一次。
        flag = 1;
        cmd->start("C:\\output.exe");
        cmd->waitForStarted();
        msg_send = "path/start/";
        Send_action();
        bool returnBool = false;
        while (returnBool == false){
            returnBool = cmd->waitForFinished(2000);
            if(!temp.contains("/")){
                temp = "path/progress/" + temp.left(temp.indexOf("\r\n"));
                socket->write(temp.toLatin1());
                socket->flush();
            }
        }
        msg_send = "path/end/";
    }else if(signal_recv == "gcode"){
        //temp = "gcode";
        flag = 1;
        msg_send = "Succeessful gcode!";
    }else if(signal_recv == "previewPath"){
        /*QString args;
        args = QString::number(layer_nr);
        sendfile->fileName = "/home/zero/function/output/layers/"+args+".cli";
        sendfile->sndMsg();

        QStringList arguments;
        arguments <<"-c"<<"/home/zero/function/infill/build/infill -s sparseInfillLineDistance=2000 -l "+args;
        cmd->start("bash",arguments);
        if(!cmd->waitForStarted()){
            qDebug() <<"failed";
        }
        cmd->waitForFinished();
        if(cmd){
              cmd->close();
        }*/
        flag = 1;
        msg_send = "previewPath/";
        layer_nr = 0;
    }else if(signal_recv == "paths"){
        flag = 1;
        QString args;
        args = QString::number(layer_nr);
        //sendfile->fileName = "/home/zero/function/output/paths/" + args + "path.cli";
        //本地测试使用
        sendfile->filename = "C:/test/server/paths/" + args + "path.cli";
        if(fileExist(sendfile->filename.toStdString())){
            sendfile->sndMsg();
            msg_send = "paths/ok";
        }else {
            msg_send = "paths/fail";
        }
        layer_nr = 0;
    }else if(signal_recv == "printSettings"){
        QStringList list = printSettings.split("|");
        for(int i = 0;i<list.size();i++){
            QStringList parameter = list[i].split("=");
            if(parameter[0] == "laserPower"){
                laserPower = parameter[1].toInt();
            }else if(parameter[0] == "scanSpeed"){
                scanSpeed = parameter[1].toInt();
            }else if(parameter[0] == "layerThickness"){
                layerThickness = parameter[1].toInt();
            }else if(parameter[0] == "scanPitch"){
                scanPitch = parameter[1].toInt();
            }else if(parameter[0] == "spotSize"){
                spotSize = parameter[1].toInt();
            }
        }
        flag = 1;
        msg_send = "printSettings/";
    }else if(signal_recv == "parallel"){
        flag = 1;
        msg_send = "parallel/" + parallelStyle;
    }else if(signal_recv == "infillStyle"){
        flag = 1;
        msg_send = "infillStyle/" + infillStyle;
    }else if(signal_recv == "targetFile"){
        flag = 1;
        msg_send = "targetFile/" + fileName;
    }else if(signal_recv == "getFileList"){
        flag = 1;
        QList<MyFile> list = sqlAction->getFilesList(pageSize,currentPage);
        QList<MyFile>::iterator listIterator = list.begin();
        //一次性返回所有文件，如果数据量非常大可能会出Bug
        //用分页查询来解决问题，最好还是重写Socket的收发，解决有可能的"粘包"问题
        msg_send = "file/" + QString::number(list.size()) + "/";
        while(listIterator != list.end()){
            msg_send = msg_send + listIterator->getName() + "|" + listIterator->getTime() + "\\";
            listIterator++;
        }
    }else if(signal_recv == "setPageSize"){
        flag = 1;
        msg_send = "setPageSize/" + QString::number(pageSize) + "/" + QString::number(sqlAction->getFilesNum());
    }
    Send_action();
    flag = 0;
}

void Server::Send_action()
{
    //获取文本框内容并以ASCII码形式发送
    if(flag==1){
        socket->write(msg_send.toLatin1());
        socket->flush();
    }
}

void Server::server_New_Connect()
{
    //获取客户端连接
    socket = server->nextPendingConnection();
    //连接QTcpSocket的信号槽，以读取新数据
    connect(socket, &QTcpSocket::readyRead, this, &Server::socket_Read_Data);
    connect(socket, &QTcpSocket::disconnected, this, &Server::socket_Disconnected);
    clientip = socket->peerAddress();
    sendfile->IP = clientip.toString();
    QStringList listip = sendfile->IP.split(":");
    sendfile->IP = listip.back();
    int port = socket->peerPort();
    qDebug() << "A Client connect!";
    qDebug() << "IP:" << listip.back() << "  Port:" << port;
}

void Server::socket_Read_Data()
{
    QByteArray buffer;
    QString str;
    //读取缓冲区数据
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
       str = tr(buffer);
    }
    msg_recv = str;
    Messageclassify();
}

void Server::socket_Disconnected()
{
    qDebug() << "A Client disconnected!";
}

void Server::on_readoutput()
{
    temp=cmd->readAllStandardOutput().data();
}

void Server::on_readerror()
{
    //QMessageBox::information(0, "Error", cmd->readAllStandardError().data());
}
