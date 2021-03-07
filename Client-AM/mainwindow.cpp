#include "mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QDataStream>
#include <QGridLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("3D打印切片软件");

   /***创建动作，菜单，工具栏*****/
    thicklayer = new QLabel("层厚(mm):");
    layernum = new QLabel("当前层:");
    tcpclient = new Tcpclient();
    showWidget =new ShowWidget(this);
    serversettings = new Serversettings();
    printsettings = new Printsettings();
    pathprogress = new Pathprogress();
    recvfile = new Recvfile();
    pathflag = false;

    setCentralWidget(showWidget);
    showWidget->textEdit_Recv->setText(" ");
    recvfile->Listen_action();
    StatusSignal("Listen succeessfully!");

    connect(tcpclient->socket, &QTcpSocket::readyRead, this, &MainWindow::RecvSignal);
    createActions();
    createMenus();
    createToolBars();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{
    openFileAction = new QAction("打开文件",this);
    openFileAction->setStatusTip("打开文件");
    sendFileAction = new QAction("发送文件",this);
    sendFileAction->setStatusTip("发送文件");

    serverSettingAction = new QAction("服务器参数设置",this);
    printerSettingAction = new QAction("打印机参数设置",this);
    cpuParalleAction = new QAction("CPU加速",this);
    cpuParalleAction->setCheckable(true);
    cpuParalleAction->setChecked(true);
    gpuParalleAction = new QAction("GPU加速",this);
    gpuParalleAction->setCheckable(true);
    pathShowAction = new QAction("路径显示",this);
    stratifyAction = new QAction("分层",this);
    previewPathAction = new QAction("路径规划",this);
    infillAction = new QAction("路径填充",this);
    generateGcodeAction = new QAction("生成G代码",this);
    connectSeverAction = new QAction("连接/断开服务器",this);
    printSettingsAction = new QAction("更新打印参数",this);
    showmodel = new QAction("显示模型",this);
    showpath = new QAction("显示路径",this);
    clearpath = new QAction("清空",this);

    layerSpinBox = new QSpinBox();
    layerSpinBox->setRange(1, 50000);
    layerSpinBox->setValue(1);
    thickSpinBox = new QDoubleSpinBox();
    thickSpinBox->setSingleStep(0.01);
    thickSpinBox->setValue(0.03);

    connect(openFileAction,&QAction::triggered,this,&MainWindow::ShowOpenFile);
    connect(sendFileAction,&QAction::triggered,this,&MainWindow::Showsend);
    connect(serverSettingAction,&QAction::triggered,this,&MainWindow::ShowServer);
    connect(printerSettingAction,&QAction::triggered,this,&MainWindow::ShowPrintSettings);
    connect(connectSeverAction,&QAction::triggered,this,&MainWindow::ConnectServer);
    connect(printSettingsAction, &QAction::triggered, this,[=](){SendSignal("printSettings/"+printsettings->getPrintSettings());});
    connect(stratifyAction, &QAction::triggered, this,&MainWindow::ShowModelList);
    connect(infillAction, &QAction::triggered, this,&MainWindow::ShowpathProgress);
    connect(generateGcodeAction, &QAction::triggered, this,[=](){SendSignal("gcode/");});
    connect(previewPathAction, &QAction::triggered, this,[=](){SendSignal("previewPath/"+QString::number(layerSpinBox->value()));});
    connect(showpath, &QAction::triggered, this,[=](){Showpaths("D:\\layers\\"+QString::number(layerSpinBox->value()));});
    connect(clearpath, &QAction::triggered, this,&MainWindow::Clearpaths);
    connect(cpuParalleAction,&QAction::triggered,this,&MainWindow::cpuParallelChecked);
    connect(gpuParalleAction,&QAction::triggered,this,&MainWindow::gpuParallelChecked);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("文件");
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(sendFileAction);

    settingMenu = menuBar()->addMenu("设置");
    settingMenu->addAction(serverSettingAction);
    settingMenu->addAction(printerSettingAction);
    //settingMenu->addAction(modelSettingAction);
    //settingMenu->setFixedSize(200,50);

    //showMenu = menuBar()->addMenu("显示");
    //showMenu->addAction(modelShowAction);
    //showMenu->addAction(pathShowAction);

    parallelMenu = menuBar()->addMenu("并行");
    parallelMenu->addAction(cpuParalleAction);
    parallelMenu->addAction(gpuParalleAction);
}

void MainWindow::createToolBars()
{
    functionTool = addToolBar("功能");
    functionTool->addAction(stratifyAction);
    functionTool->addAction(infillAction);
    functionTool->addAction(generateGcodeAction);
    functionTool->addAction(connectSeverAction);
    functionTool->addAction(printSettingsAction);
    functionTool->setAllowedAreas(Qt::RightToolBarArea);
    functionTool->setFixedHeight(50);

    //modelTool = addToolBar("模型");
    //modelTool->setAllowedAreas(Qt::LeftToolBarArea);

    pathTool = addToolBar("路径");
    //pathTool->addAction(showmodel);
    pathTool->addAction(showpath);
    pathTool->addAction(clearpath);
    pathTool->addAction(previewPathAction);
    QLabel *tmplabel = new QLabel();
    tmplabel->setFixedWidth(10);
    //pathTool->addWidget(tmplabel);
    pathTool->addWidget(layernum);
    pathTool->addWidget(layerSpinBox);
    QLabel *tmplabel2 = new QLabel();
    tmplabel2->setFixedWidth(15);
    pathTool->addWidget(tmplabel2);
    pathTool->addWidget(thicklayer);
    pathTool->addWidget(thickSpinBox);
    pathTool->addAction(previewPathAction);
}

void MainWindow::cpuParallelChecked()
{
    if(tcpclient->getTcpStatus()){
        cpuParalleAction->setChecked(true);
        gpuParalleAction->setChecked(false);
        //向后台更新参数
        SendSignal("parallel/cpu");
    }else{
        //未连接服务器，还原原本的打勾状态
        cpuParalleAction->setChecked(!cpuParalleAction->isChecked());
        QMessageBox::warning(NULL, "警告", "请先连接服务器", QMessageBox::Ok , QMessageBox::Ok);
    }
}

void MainWindow::gpuParallelChecked()
{
    if(tcpclient->getTcpStatus()){
        cpuParalleAction->setChecked(false);
        gpuParalleAction->setChecked(true);
        //向后台更新参数
        SendSignal("parallel/gpu");
    }else{
        //未连接服务器，还原原本的打勾状态
        gpuParalleAction->setChecked(!gpuParalleAction->isChecked());
        QMessageBox::warning(NULL, "警告", "请先连接服务器", QMessageBox::Ok , QMessageBox::Ok);
    }
}

void MainWindow::ShowModelList()
{
    modellist = new ModelList();
    modellist->initModelList(10);
    modellist->exec();
    delete modellist;
    modellist = nullptr;
    //SendSignal("slicing/"+QString::number(thickSpinBox->value()));
}

void MainWindow::ShowOpenFile()
{
    QString s = QFileDialog::getOpenFileName(this,"open file dialog","/","STL files(*.STL);;stl files(*.stl)");
    filename = s;
    loadFile(filename);
}

void MainWindow::loadFile(QString filename)
{
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
    }
}

void MainWindow::Showpaths(QString input)
{
    QString input_layer, input_path;
    input_layer = input + ".cli";
    input_path = input + "path.cli";
    showWidget->showpath->clearpath();
    //showWidget->showpath->plotlayer(input_layer);
    showWidget->showpath->plotpath(input_path);
}

void MainWindow::Clearpaths()
{
    showWidget->showpath->clearpath();
}

void MainWindow::ShowServer()
{
   serversettings->exec();
}

void MainWindow::ShowPrintSettings()
{
   printsettings->exec();
}

void MainWindow::Showsend()
{
    if(tcpclient->getTcpStatus()){
        sendfile = new SendFile();
        sendfile->exec();
        sendfile->IP = serversettings->ip;
    }else{
        QMessageBox::warning(NULL, "警告", "请先连接服务器", QMessageBox::Ok , QMessageBox::Ok);
    }
}

void MainWindow::ShowpathProgress()
{
    if(tcpclient->getTcpStatus()){
        if(pathflag){
            pathprogress->setModal(true);
            pathprogress->show();
            SendSignal("infill/");
            pathflag = false;
        }else{
            QMessageBox::warning(NULL, "警告", "请先进行分层", QMessageBox::Ok , QMessageBox::Ok);
        }
    }else{
        QMessageBox::warning(NULL, "警告", "请先连接服务器", QMessageBox::Ok , QMessageBox::Ok);
    }

}

void MainWindow::ConnectServer()
{
    if(!tcpclient->getTcpStatus()){
        tcpclient->IP = serversettings->ip;
        tcpclient->Port = serversettings->port;
        tcpclient->on_pushButton_Connect_clicked();
        if(tcpclient->getTcpStatus()){
            StatusClear();
            StatusSignal("Server connected!");
        }else{
            QMessageBox::warning(NULL, "警告", "服务器连接超时", QMessageBox::Ok , QMessageBox::Ok);
        }
    }else{
        tcpclient->socket_Disconnected();
        StatusSignal("Server disconnected!");
    }
}


// 向服务端发送消息
void MainWindow::SendSignal(QString msg)
{
    if(tcpclient->getTcpStatus()){
        tcpclient->msg_send = msg;
        tcpclient->on_pushButton_Send_clicked();
    }else{
        QMessageBox::warning(NULL, "警告", "请先连接服务器", QMessageBox::Ok , QMessageBox::Ok);
    }
}

// 接收服务端返回的信息
void MainWindow::RecvSignal()
{
   if(tcpclient->msg_flag!=0){
       if(tcpclient->msg_recv=="Succeessful Pathplanning!"){
            tcpclient->msg_send = "paths/"+QString::number(layerSpinBox->value());
            tcpclient->on_pushButton_Send_clicked();
            StatusSignal("Succeessful Receive Polygon!");
       }else if(tcpclient->msg_recv.left(4)=="path"){
            QStringList list= tcpclient->msg_recv.split(" ");
            pathprogress->log("正在对"+ list[list.size()-1] +"层进行填充 , 请稍候 !");
            pathprogress->updateProgress(list[list.size()-1].toInt());
       }
       // 若返回切片的层数
       else if(tcpclient->msg_recv.contains("total")){
           QStringList list= tcpclient->msg_recv.split(" ");
           QString tips = "当前层（最大" + list[list.size() - 2] + "）";
           layernum->setText(tips);
           layermax = list[list.size() - 2].toInt();
           layerSpinBox->setRange(1, layermax);
           pathprogress->initProgress(layermax);
           pathprogress->log("开始准备进行填充 , 总共有"+ list[list.size() - 2] +"层 ! ");
           pathflag = true;
           StatusSignal(tcpclient->msg_recv);
        }
       else{
           StatusSignal(tcpclient->msg_recv);
       }
   }
}

// 状态输出栏
void MainWindow::StatusSignal(QString msg)
{
   showWidget->textEdit_Recv->append(msg);
   QTextCursor cursor = showWidget->textEdit_Recv->textCursor();
   cursor.movePosition(QTextCursor::End);
   showWidget->textEdit_Recv->setTextCursor(cursor);
}

void MainWindow::StatusClear()
{
    showWidget->textEdit_Recv->clear();
}



