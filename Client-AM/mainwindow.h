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
#include <sys/stat.h>
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
    /**菜单栏**/
    QMenu *fileMenu;
    QMenu *settingMenu;
    QMenu *parallelMenu;
    QMenu *infillMenu;
    /**文件菜单项**/
    QAction *openFileAction;
    QAction *sendFileAction;
    /**设置菜单项**/
    QAction *serverSettingAction;
    QAction *printerSettingAction;
    /**并行菜单项**/
    QAction *cpuParallelAction;
    QAction *gpuParallelAction;
    /**填充菜单项**/
    QAction *hatchInfillAction;
    QAction *offsetInfilllAction;


    /****工具栏****/
    QToolBar *functionTool;
    QToolBar *pathTool;

    /*******功能*********/
    //连接或断开服务器
    QAction *connectSeverAction;
    //打印参数设置
    QAction *printSettingsAction;
    //分层
    QAction *stratifyAction;
    //填充
    QAction *infillAction;
    //Gcode生成
    QAction *generateGcodeAction;
    //路径规划
    QAction *previewPathAction;
    //显示路径
    QAction *showpath;
    //清空
    QAction *clearpath;
    //当前层
    QSpinBox *layerSpinBox;
    QLabel *layernum;
    //层厚
    QDoubleSpinBox *thickSpinBox;
    QLabel *thicklayer;

    //操作流程进度
    Processnode processnode;
    //服务器与打印参数设置界面
    Serversettings *serversettings;
    Printsettings *printsettings;
    //IP地址与端口
    QString IP;
    quint16 port;
    //进度条
    Pathprogress *pathprogress;
    //文件列表界面
    ModelList *modellist;
    //TCP交互
    Tcpclient *tcpclient;
    //状态输出栏
    ShowWidget *showWidget;
    //收发文件
    SendFile *sendfile;
    Recvfile *recvfile;
    //切片的层数
    int layerNumber;
    QTime infillTime;

protected slots:
    void cpuParallelChecked();
    void gpuParallelChecked();
    void hatchInfillChecked();
    void offsetInfillChecked();
    bool getTcpStatus();
    void sliceAction();
    void gcodeAction();
    void getPreviewPathAction();
    void setPrintSettings();
    void getPathsAction();
    void setPageSizeAction();
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

//使用stat函数获取文件状态，成功则存在，否则不存在,比较几种方式，用stat() 函数的性能最好
inline bool fileExist(const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}
#endif // MAINWINDOW_H
