#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QComboBox>
#include <QSpinBox>
#include <QFontComboBox>
#include <QToolBar>
#include <QToolButton>
#include <QTextCharFormat>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "serversettings.h"
#include "printsettings.h"
#include "tcpclient.h"
#include "showwidget.h"
#include "sendfile.h"
#include "recvfile.h"
#include "pathprogress.h"
#include "modellist.h"

//枚举类型，记录几个阶段 1.选择模型 2.分层 3.路径填充 4.其他操作
enum class Processnode{INIT,CHOOSEFILE,SLICE,INFILL,OTHERS};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createActions();
    void createMenus();
    void createToolBars();
    void mergeFormat(QTextCharFormat);
private:
    QMenu *fileMenu;
    QMenu *settingMenu;
    //QMenu *showMenu;
    QMenu *parallelMenu;

    /**文件菜单项**/
    QAction *openFileAction;
    QAction *sendFileAction;
    /**设置菜单项**/
    //QAction *settingAction;
    QAction *serverSettingAction;
    QAction *modelSettingAction;
    /**显示菜单项**/
    //QAction *showAction;
    QAction *modelShowAction;
    QAction *pathShowAction;
    /**并行菜单项**/
    //QAction *paralleAction;
    QAction *cpuParalleAction;
    QAction *gpuParalleAction;
    /****工具栏****/
    QToolBar *functionTool;
    //QToolBar *modelTool;
    QToolBar *pathTool;
    /*******功能*********/
    QAction *stratifyAction;
    QAction *previewPathAction;
    QAction *infillAction;
    QAction *generateGcodeAction;
    QAction *connectSeverAction;
    QAction *printerSettingAction;
    QAction *printSettingsAction;
    QAction *showmodel;
    QAction *showpath;
    QAction *clearpath;

    QSpinBox *layerSpinBox;
    QDoubleSpinBox *thickSpinBox;
    QLabel *thicklayer;
    QLabel *layernum;

    QString filename;
    QString IP;
    bool pathflag;
    quint16 port;
    Processnode processnode;
    Serversettings *serversettings;
    Printsettings *printsettings;
    Pathprogress *pathprogress;
    ModelList *modellist;
    Tcpclient *tcpclient;
    ShowWidget *showWidget;
    SendFile *sendfile;
    Recvfile *recvfile;


protected slots:
    void cpuParallelChecked();
    void gpuParallelChecked();
    bool getTcpStatus();
    void sliceAction();
    void gcodeAction();
    void getPreviewPathAction();
    void setPrintSettings();
    void ShowOpenFile();
    void Showpaths();
    void ShowpathProgress();
    void Clearpaths();
    void ShowServer();
    void ShowPrintSettings();
    void Showsend();
    void ConnectServer();
    void SendSignal(QString msg);
    void RecvSignal();
    void StatusSignal(QString msg);
    void StatusClear();
    bool processcheck(Processnode process);
    void showMessageBox(QString msg);
};
#endif // MAINWINDOW_H
