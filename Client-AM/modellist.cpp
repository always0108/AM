#include "modellist.h"


ModelList::ModelList(QDialog *parent) : QDialog(parent)
{
    setWindowTitle("选择模型");
    setFixedSize(300,200);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
}

void ModelList::initModelList(int fileNum,int pageSize,Tcpclient *tcpclient)
{
    this->pageSize = pageSize;
    this->tcpclient = tcpclient;
    totalPage = (fileNum + pageSize - 1) / pageSize;
    currentPage = 1;
    tableView = new QTableView();
    model = new QStandardItemModel();
    /* 设置列数 */
    model->setColumnCount(2);
    /* 设置行数 */
    model->setRowCount(pageSize);
    tableView->setModel(model);
    /* 行颜色交替显示 */
    tableView->setAlternatingRowColors(true);
    /* 在表格内加入内容 */
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->horizontalHeader()->setVisible(false); //设置水平表头不可见
    //tableView->verticalHeader()->setVisible(false); //设置垂直头不可见
    tableView->verticalHeader()->setDefaultSectionSize(10); //设置行高
    tableView->setFrameShape(QFrame::NoFrame); //设置无边框
    tableView->setShowGrid(false); //设置不显示格子线
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    tableView->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色

    /*tableView->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");
    */

    tableView->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");
    count = 0;
    current = new QLabel(QString::number(currentPage));
    space = new QLabel("/");
    total = new QLabel(QString::number(totalPage));
    prePage = new QPushButton("上一页");
    nextPage = new QPushButton("下一页");
    pageLayout = new QHBoxLayout();
    pageLayout->addStretch();
    pageLayout->addWidget(current);
    pageLayout->addWidget(space);
    pageLayout->addWidget(total);
    pageLayout->addStretch();
    pageLayout->addWidget(prePage);
    pageLayout->addWidget(nextPage);
    choosenote = new QLabel("未选择文件");
    filename = new QLabel("");
    noteLayout = new QHBoxLayout();
    noteLayout->addWidget(choosenote);
    noteLayout->addWidget(filename);
    noteLayout->addStretch();
    confirmBtn = new QPushButton("确定");
    confirmBtn->setEnabled(false);
    confirmBtn->setFocusPolicy(Qt::NoFocus);
    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(tableView,0,0);
    mainLayout->addLayout(pageLayout,1,0);
    mainLayout->addLayout(noteLayout,2,0);
    mainLayout->addWidget(confirmBtn,3,0);
    mainLayout->setMargin(5);
    mainLayout->setSpacing(5);
    connect(tableView,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(choosefile(const QModelIndex &)));
    connect(prePage,SIGNAL(clicked()),this,SLOT(pre()));
    connect(nextPage,SIGNAL(clicked()),this,SLOT(next()));
    connect(confirmBtn,SIGNAL(clicked()),this,SLOT(onClicked()));
    update();
}

void ModelList::choosefile(const QModelIndex &index)
{
    choosenote->setText("已选择：");
    QStandardItem *item = model->item(index.row(),0);
    filename->setText(item->text());
    confirmBtn->setEnabled(true);
}

void ModelList::onClicked()
{
    sendSignal("targetFile/" + filename->text());
    this->accept();
}

void ModelList::updateData(int size , QString list)
{
    model->removeRows(0, model->rowCount());
    current->setText(QString::number(currentPage));
    QStringList filelist = list.split("\\");
    for(int i = 0; i < size; i++){
        QStringList tmp = filelist[i].split("|");
        model->insertRow(i);
        model->setItem(i, 0, new QStandardItem(tmp[0]));
        model->setItem(i, 1, new QStandardItem(tmp[1]));
    }
}

void ModelList::pre()
{
    if(currentPage > 1){
        currentPage--;
        update();
    }else{
        showMessageBox("已经是最前一页");
    }
}

void ModelList::next()
{
    if(currentPage < totalPage){
        currentPage++;
        update();
    }else{
        showMessageBox("已经是最后一页");
    }
}

void ModelList::update()
{
    sendSignal("getFileList/" + QString::number(currentPage));
}

void ModelList::sendSignal(QString msg)
{
    if(tcpclient->getTcpStatus()){
        tcpclient->msg_send = msg;
        tcpclient->on_pushButton_Send_clicked();
    }else{
        showMessageBox("服务器未连接");
    }
}

void ModelList::showMessageBox(QString msg)
{
    QMessageBox::warning(this, "警告", msg , QMessageBox::Ok , QMessageBox::Ok);
}


