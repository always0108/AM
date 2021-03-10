#include "mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QDataStream>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //设置标题
    setWindowTitle("3D打印切片软件");

    //创建所需对象
    tcpclient = new Tcpclient();
    showWidget = new ShowWidget(this);
    serversettings = new Serversettings();
    printsettings = new Printsettings();
    pathprogress = new Pathprogress();
    recvfile = new Recvfile();
    processnode = Processnode::INIT;

    //初始化状态输出栏
    setCentralWidget(showWidget);
    StatusClear();

    //监听接收文件socket
    recvfile->Listen_action();
    connect(tcpclient->socket, &QTcpSocket::readyRead, this, &MainWindow::RecvSignal);
    StatusSignal("端口已成功监听");

    createActions();
    createMenus();
    createToolBars();
}

MainWindow::~MainWindow()
{
    delete tcpclient;
    delete showWidget;
    delete serversettings;
    delete printsettings;
    delete pathprogress;
    delete recvfile;
}

void MainWindow::createActions()
{
    //菜单栏
    openFileAction = new QAction("选择已有文件",this);
    sendFileAction = new QAction("添加新的文件",this);
    serverSettingAction = new QAction("服务器参数设置",this);
    printerSettingAction = new QAction("打印机参数设置",this);
    cpuParalleAction = new QAction("CPU加速",this);
    gpuParalleAction = new QAction("GPU加速",this);
    cpuParalleAction->setCheckable(true);
    cpuParalleAction->setChecked(true);
    gpuParalleAction->setCheckable(true);

    //工具栏
    connectSeverAction = new QAction("连接/断开服务器",this);
    printSettingsAction = new QAction("更新打印参数",this);
    stratifyAction = new QAction("分层",this);
    previewPathAction = new QAction("路径规划",this);
    generateGcodeAction = new QAction("生成G代码",this);
    infillAction = new QAction("路径填充",this);
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
    connect(printSettingsAction, &QAction::triggered, this,&MainWindow::setPrintSettings);
    connect(stratifyAction, &QAction::triggered, this,&MainWindow::sliceAction);
    connect(infillAction, &QAction::triggered, this,&MainWindow::ShowpathProgress);
    connect(generateGcodeAction, &QAction::triggered, this,&MainWindow::gcodeAction);
    connect(previewPathAction, &QAction::triggered, this,&MainWindow::getPreviewPathAction);
    connect(showpath, &QAction::triggered, this,&MainWindow::Showpaths);
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

    parallelMenu = menuBar()->addMenu("并行");
    parallelMenu->addAction(cpuParalleAction);
    parallelMenu->addAction(gpuParalleAction);
}

void MainWindow::createToolBars()
{
    //左侧工具栏
    functionTool = addToolBar("功能");
    functionTool->addAction(connectSeverAction);
    functionTool->addAction(printSettingsAction);
    functionTool->addAction(stratifyAction);
    functionTool->addAction(infillAction);
    functionTool->addAction(generateGcodeAction);
    functionTool->setAllowedAreas(Qt::RightToolBarArea);
    functionTool->setFixedHeight(50);

    //右侧工具栏
    pathTool = addToolBar("路径");
    pathTool->addAction(previewPathAction);
    pathTool->addAction(showpath);
    pathTool->addAction(clearpath);
    layernum = new QLabel("当前层:");
    pathTool->addWidget(layernum);
    pathTool->addWidget(layerSpinBox);
    thicklayer = new QLabel("  层厚(mm):");
    pathTool->addWidget(thicklayer);
    pathTool->addWidget(thickSpinBox);
}

void MainWindow::cpuParallelChecked()
{
    if(getTcpStatus()){
        cpuParalleAction->setChecked(true);
        gpuParalleAction->setChecked(false);
        //向后台更新参数
        SendSignal("parallel/cpu");
    }else{
        //未连接服务器，还原原本的打勾状态
        cpuParalleAction->setChecked(!cpuParalleAction->isChecked());
    }
}

void MainWindow::gpuParallelChecked()
{
    if(getTcpStatus()){
        cpuParalleAction->setChecked(false);
        gpuParalleAction->setChecked(true);
        //向后台更新参数
        SendSignal("parallel/gpu");
    }else{
        //未连接服务器，还原原本的打勾状态
        gpuParalleAction->setChecked(!gpuParalleAction->isChecked());
    }
}

bool MainWindow::getTcpStatus()
{
    if(tcpclient->getTcpStatus()){
        return true;
    }else{
        showMessageBox("请先连接服务器");
        return false;
    }
}


void MainWindow::sliceAction()
{
    if(getTcpStatus()  && processcheck(Processnode::CHOOSEFILE)){
        SendSignal("slicing/"+QString::number(thickSpinBox->value()));
    }
}

void MainWindow::gcodeAction()
{
    if(getTcpStatus()  && processcheck(Processnode::INFILL)){
        SendSignal("gcode/");
    }
}

void MainWindow::getPreviewPathAction()
{
    if(getTcpStatus()  && processcheck(Processnode::INFILL)){
        SendSignal("previewPath/"+QString::number(layerSpinBox->value()));
    }
}

void MainWindow::setPrintSettings()
{   if(getTcpStatus()){
        SendSignal("printSettings/"+printsettings->getPrintSettings());
    }
}

void MainWindow::ShowOpenFile()
{
    if(getTcpStatus() && processcheck(Processnode::INIT)){
        SendSignal("getFileList/");
    }
}

void MainWindow::Showpaths()
{
    if(getTcpStatus() && processcheck(Processnode::INFILL)){
        QString input = "D:\\layers\\"+QString::number(layerSpinBox->value());
        QString input_layer, input_path;
        input_layer = input + ".cli";
        input_path = input + "path.cli";
        showWidget->showpath->clearpath();
        //showWidget->showpath->plotlayer(input_layer);
        showWidget->showpath->plotpath(input_path);
    }
}

void MainWindow::ShowpathProgress()
{
    if(getTcpStatus() && processcheck(Processnode::SLICE)){
        pathprogress->setModal(true);
        pathprogress->show();
        SendSignal("infill/");
    }
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
    if(getTcpStatus()){
        sendfile = new SendFile();
        sendfile->IP = serversettings->ip;
        sendfile->exec();
        delete sendfile;
        sendfile = nullptr;
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
            StatusSignal("服务器已连接");
        }else{
            showMessageBox("服务器连接超时");
        }
    }else{
        tcpclient->socket_Disconnected();
        StatusSignal("服务器连接已断开");
    }
}


// 向服务端发送消息
void MainWindow::SendSignal(QString msg)
{
    if(getTcpStatus()){
        tcpclient->msg_send = msg;
        tcpclient->on_pushButton_Send_clicked();
    }
}

// 接收服务端返回的信息
void MainWindow::RecvSignal()
{
   if(tcpclient->msg_flag!=0){
       if(tcpclient->msg_recv.contains("/")){
           QStringList list = tcpclient->msg_recv.split("/");
           // 若返回切片的层数
           if(list[0]=="slice"){
               QString tips = "当前层（最大" + list[1] + "）";
               layernum->setText(tips);
               layerSpinBox->setRange(1, list[1].toInt());
               pathprogress->initProgress(list[1].toInt());
               pathprogress->log("开始准备进行填充 , 总共有"+ list[1] +"层 ! ");
               processnode = Processnode::SLICE;
               StatusSignal("切片完成 , 总共有" + list[1] + "层 !");
           }else if(list[0] == "path"){
               if(list[1] == "progress"){
                   pathprogress->log("正在对"+ list[2] +"层进行填充 , 请稍候 !");
                   pathprogress->updateProgress(list[2].toInt());
               }else if(list[1] == "start"){
                   StatusSignal("开始进行路径填充");
               }else if(list[1] == "end"){
                   processnode = Processnode::INFILL;
                   StatusSignal("路径填充已经结束");
               }
           }else if(list[0] == "targetFile"){
               processnode = Processnode::CHOOSEFILE;
               StatusSignal("目标文件已设置为 " + list[1]);
           }else if(list[0] == "file"){
               //StatusSignal(tcpclient->msg_recv);
               modellist = new ModelList();
               int size = list[1].toInt();
               modellist->initModelList(size);
               QStringList filelist = list[2].split("\\");
               for(int i = 0; i < size; i++){
                   QStringList tmp = filelist[i].split("|");
                   modellist->insertData(tmp[0],tmp[1]);
               }
               if(modellist->exec() == QDialog::Accepted){
                   filename = modellist->getTargetfile();
                   SendSignal("targetFile/" + filename);
               }
               delete modellist;
               modellist = nullptr;
           }
       }else{
           if(tcpclient->msg_recv=="Succeessful Pathplanning!"){
                tcpclient->msg_send = "paths/"+QString::number(layerSpinBox->value());
                tcpclient->on_pushButton_Send_clicked();
                StatusSignal("Succeessful Receive Polygon!");
           }
           else{
               StatusSignal(tcpclient->msg_recv);
           }
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

bool MainWindow::processcheck(Processnode process)
{
    if(processnode < process){
        QString msg;
        if(processnode == Processnode::INIT){
            msg = "请选择已有文件";
        }else if(processnode == Processnode::CHOOSEFILE){
            msg = "请进行分层操作";
        }else if(processnode == Processnode::SLICE){
            msg = "请进行路径填充";
        }
        showMessageBox(msg);
        return false;
    }else{
        return true;
    }
}

void MainWindow::showMessageBox(QString msg)
{
    QMessageBox::warning(NULL, "警告", msg, QMessageBox::Ok , QMessageBox::Ok);
}




