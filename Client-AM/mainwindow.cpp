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
    showWidget = new ShowWidget(this);
    recvfile = new Recvfile();
    recvfile->setShowWidget(showWidget);
    tcpclient = new Tcpclient();
    serversettings = new Serversettings();
    printsettings = new Printsettings();
    pathprogress = new Pathprogress();
    processnode = Processnode::INIT;
    modellist = nullptr;

    //初始化日志输出界面
    setCentralWidget(showWidget);
    StatusClear();

    //监听接收文件socket
    recvfile->Listen_action();
    connect(tcpclient->socket, &QTcpSocket::readyRead, this, &MainWindow::RecvSignal);
    StatusSignal("端口已成功监听");

    //创建菜单栏、功能栏等
    createActions();
    createMenus();
    createToolBars();
}

MainWindow::~MainWindow()
{
    //没有指定父对象并且没有添加到界面显示的，需要手动释放
    delete recvfile;
    delete tcpclient;
    delete serversettings;
    delete printsettings;
    delete pathprogress;
    delete modellist;
}

void MainWindow::createActions()
{
    //菜单栏
    openFileAction = new QAction("选择已有文件",this);
    sendFileAction = new QAction("添加新的文件",this);
    serverSettingAction = new QAction("服务器参数设置",this);
    printerSettingAction = new QAction("打印机参数设置",this);
    cpuParallelAction = new QAction("CPU加速",this);
    gpuParallelAction = new QAction("GPU加速",this);
    cpuParallelAction->setCheckable(true);
    cpuParallelAction->setChecked(true);
    gpuParallelAction->setCheckable(true);
    gpuParallelAction->setChecked(false);
    hatchInfillAction = new QAction("Hatch",this);
    offsetInfilllAction = new QAction("Offset",this);
    hatchInfillAction->setCheckable(true);
    hatchInfillAction->setChecked(true);
    offsetInfilllAction->setCheckable(true);
    offsetInfilllAction->setChecked(false);

    //工具栏
    connectSeverAction = new QAction("连接/断开服务器",this);
    printSettingsAction = new QAction("更新打印参数",this);
    stratifyAction = new QAction("分层",this);
    previewPathAction = new QAction("实时路径规划",this);
    generateGcodeAction = new QAction("生成G代码",this);
    infillAction = new QAction("路径规划",this);
    showpath = new QAction("显示路径",this);
    clearpath = new QAction("清空",this);
    layerSpinBox = new QSpinBox();
    layerSpinBox->setRange(1, 50000);
    layerSpinBox->setValue(1);
    thickSpinBox = new QDoubleSpinBox();
    thickSpinBox->setSingleStep(0.01);
    thickSpinBox->setValue(0.03);

    connect(openFileAction,&QAction::triggered,this,&MainWindow::setPageSizeAction);
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
    connect(cpuParallelAction,&QAction::triggered,this,&MainWindow::cpuParallelChecked);
    connect(gpuParallelAction,&QAction::triggered,this,&MainWindow::gpuParallelChecked);
    connect(hatchInfillAction,&QAction::triggered,this,&MainWindow::hatchInfillChecked);
    connect(offsetInfilllAction,&QAction::triggered,this,&MainWindow::offsetInfillChecked);
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
    parallelMenu->addAction(cpuParallelAction);
    parallelMenu->addAction(gpuParallelAction);

    infillMenu = menuBar()->addMenu("填充方式");
    infillMenu->addAction(hatchInfillAction);
    infillMenu->addAction(offsetInfilllAction);
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

/************* 菜单栏打勾 **************/
void MainWindow::cpuParallelChecked()
{
    if(getTcpStatus()){
        // 判断状态是否改变，若改变才更新
        if(cpuParallelAction->isChecked() || gpuParallelAction->isChecked())
            SendSignal("parallel/CPU");
        cpuParallelAction->setChecked(true);
        gpuParallelAction->setChecked(false);
    }else{
        //未连接服务器，还原原本的打勾状态
        cpuParallelAction->setChecked(!cpuParallelAction->isChecked());
    }
}

void MainWindow::gpuParallelChecked()
{
    if(getTcpStatus()){
        if(cpuParallelAction->isChecked() || gpuParallelAction->isChecked())
            SendSignal("parallel/GPU");
        cpuParallelAction->setChecked(false);
        gpuParallelAction->setChecked(true);
    }else{
        gpuParallelAction->setChecked(!gpuParallelAction->isChecked());
    }
}

void MainWindow::hatchInfillChecked()
{
    if(getTcpStatus()){
        if(hatchInfillAction->isChecked() || offsetInfilllAction->isChecked())
            SendSignal("infillStyle/INFILL_LINES");
        hatchInfillAction->setChecked(true);
        offsetInfilllAction->setChecked(false);
    }else{
        hatchInfillAction->setChecked(!hatchInfillAction->isChecked());
    }
}

void MainWindow::offsetInfillChecked()
{
    if(getTcpStatus()){
        if(hatchInfillAction->isChecked() || offsetInfilllAction->isChecked())
            SendSignal("infillStyle/INFILL_CONCENTRIC");
        hatchInfillAction->setChecked(false);
        offsetInfilllAction->setChecked(true);
    }else{
        offsetInfilllAction->setChecked(!offsetInfilllAction->isChecked());
    }
}

/************* TCP连接状态 **************/
bool MainWindow::getTcpStatus()
{
    if(tcpclient->getTcpStatus()){
        return true;
    }else{
        showMessageBox("请先连接服务器");
        return false;
    }
}

/************* 一些请求后台Action **************/
void MainWindow::sliceAction()
{
    if(getTcpStatus()  && processcheck(Processnode::CHOOSEFILE)){
        SendSignal("slicing/" + QString::number(thickSpinBox->value()));
    }
}

void MainWindow::gcodeAction()
{
    if(getTcpStatus()  && processcheck(Processnode::SLICE)){
        SendSignal("gcode/");
    }
}

void MainWindow::getPreviewPathAction()
{
    if(getTcpStatus()  && processcheck(Processnode::SLICE)){
        SendSignal("previewPath/" + QString::number(layerSpinBox->value()));
    }

}

void MainWindow::setPrintSettings()
{   if(getTcpStatus()){
        SendSignal("printSettings/" + printsettings->getPrintSettings());
    }
}

void  MainWindow::getPathsAction()
{
    if(getTcpStatus()  && processcheck(Processnode::INFILL)){
        SendSignal("paths/" + QString::number(layerSpinBox->value()));
    }
}

void MainWindow::setPageSizeAction()
{
    if(getTcpStatus() && processcheck(Processnode::INIT)){
        //设置分页大小为4
        SendSignal("setPageSize/4");
    }
}

/**************** 路径 *****************/
// 1.显示路径
void MainWindow::Showpaths()
{
    if(getTcpStatus() && processcheck(Processnode::INFILL)){
        QString input = "D:\\layers\\" + QString::number(layerSpinBox->value());
        QString input_path = input + "path.cli";
        // 目前感觉这个逻辑上还是有些问题，有可能不是最新的文件
        // 有待商榷
        if(fileExist(input_path.toStdString())){
            showWidget->showpath->clearpath();
            showWidget->showpath->plotpath(input_path);
            StatusSignal("路径已展示");
        }else{
            //去服务端获取文件
            StatusSignal("本地文件不存在");
            StatusSignal("正在从服务器获取");
            getPathsAction();
        }
    }
}

// 2.清空当前路径
void MainWindow::Clearpaths()
{
    showWidget->showpath->clearpath();
    StatusSignal("路径已清空");
}

/************ 显示设置界面 ************/
// 1.服务器设置
void MainWindow::ShowServer()
{
    serversettings->exec();
}

// 2. 打印参数设置
void MainWindow::ShowPrintSettings()
{
    printsettings->exec();
}

/************ 显示发送文件界面 ************/
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

/********** 填充进度条 ***********/
void MainWindow::ShowpathProgress()
{
    if(getTcpStatus() && processcheck(Processnode::SLICE)){
        pathprogress->initProgress(layerNumber);
        infillTime.start();
        pathprogress->log("开始准备进行填充 , 总共有"+ QString::number(layerNumber) +"层 ! ");
        pathprogress->setModal(true);
        pathprogress->show();
        SendSignal("infill/");
    }
}

/********** 连接服务器 ***********/
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

/************ 发送消息 ************/
void MainWindow::SendSignal(QString msg)
{
    if(getTcpStatus()){
        tcpclient->msg_send = msg;
        tcpclient->on_pushButton_Send_clicked();
    }
}

/************ 消息分发 ************/
void MainWindow::RecvSignal()
{
   if(tcpclient->msg_flag!=0){
       if(tcpclient->msg_recv.contains("/")){
           QStringList list = tcpclient->msg_recv.split("/");
           // 若返回切片的层数
           if(list[0]=="slice"){
               QString tips = "当前层（最大" + list[1] + "）";
               layernum->setText(tips);
               layerNumber = list[1].toInt();
               layerSpinBox->setRange(1, layerNumber);
               processnode = Processnode::SLICE;
               StatusSignal("分层完成, 耗时" +list[2] + "s, 总共有" + list[1] + "层 !");
           }else if(list[0] == "path"){
               if(list[1] == "progress"){
                   pathprogress->log("正在对"+ list[2] +"层进行填充 , 请稍候 !");
                   pathprogress->updateProgress(list[2].toInt());
               }else if(list[1] == "start"){
                   StatusSignal("开始进行路径填充");
               }else if(list[1] == "end"){
                   processnode = Processnode::INFILL;
                   pathprogress->setStatus(false);
                   int timeElapsed = infillTime.elapsed();
                   pathprogress->log("耗时"+QString::number((timeElapsed*1.0)/1000,'f',2)+"s ！");
                   StatusSignal("路径填充已经结束, 耗时"+QString::number((timeElapsed*1.0)/1000,'f',2)+"s");
               }
           }else if(list[0] == "targetFile"){
               processnode = Processnode::CHOOSEFILE;
               StatusSignal("目标文件已设置为 " + list[1]);
           }else if(list[0] == "setPageSize"){
               if(modellist != nullptr){
                   delete modellist;
                   modellist = nullptr;
               }
               modellist = new ModelList();
               int pageSize = list[1].toInt();
               int fileNum = list[2].toInt();
               modellist->initModelList(fileNum,pageSize,tcpclient);
               modellist->setModal(true);
               modellist->show();
           }else if(list[0] == "file"){
               modellist->updateData(list[1].toInt(),list[2]);
           }else if(list[0] == "previewPath"){
               StatusSignal("路径规划"+list[1]+"层完成，请点击“显示路径”按钮");
               processnode = Processnode::INFILL;
           }else if(list[0] == "paths"){
               if(list[1] == "fail"){
                   showMessageBox("没有该层的路径文件");
               }
           }else if(list[0] == "parallel"){
               StatusSignal("加速方式已设置为" + list[1] + "加速");
           }else if(list[0] == "infillStyle"){
               if(list[1] == "INFILL_CONCENTRIC"){
                   StatusSignal("填充方式已设置为Offset");
               }else if(list[1] == "INFILL_LINES"){
                   StatusSignal("填充方式已设置为Hatch");
               }
           }else if(list[0] == "printSettings"){
               StatusSignal("打印参数设置成功");
           }
       }else{
           StatusSignal(tcpclient->msg_recv);
       }
   }
}

/************ 日志输出 ************/
// 1.输出信息
void MainWindow::StatusSignal(QString msg)
{
    showWidget->log(msg);
}

// 2.清屏
void MainWindow::StatusClear()
{
    showWidget->textEdit_Recv->clear();
}

/************ 流程检查 ************/
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

/************ 消息弹窗 ************/
void MainWindow::showMessageBox(QString msg)
{
    QMessageBox::warning(this, "警告", msg, QMessageBox::Ok , QMessageBox::Ok);
}




