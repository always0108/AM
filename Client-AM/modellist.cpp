#include "modellist.h"


ModelList::ModelList(QDialog *parent) : QDialog(parent)
{
    setWindowTitle("选择模型");
    setFixedSize(300,200);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
}

void ModelList::initModelList(int size)
{
    tableView = new QTableView();
    model = new QStandardItemModel();
    /* 设置列数 */
    model->setColumnCount(1);
    /* 设置行数 */
    model->setRowCount(size);
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
    for(int i = 0; i < size; i++){
        model->setItem(i, 0, new QStandardItem("model" + QString::number(i+1,10) + ".stl"));
    }
    choosenote = new QLabel("未选择文件");
    filename = new QLabel("");
    noteLayout = new QHBoxLayout();
    noteLayout->addWidget(choosenote);
    noteLayout->addWidget(filename);
    confirmBtn = new QPushButton("确定");
    confirmBtn->setEnabled(false);
    confirmBtn->setFocusPolicy(Qt::NoFocus);
    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(tableView,0,0);
    mainLayout->addLayout(noteLayout,1,0);
    mainLayout->addWidget(confirmBtn,2,0);
    mainLayout->setMargin(5);
    mainLayout->setSpacing(5);
    connect(tableView,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(choosefile(const QModelIndex &)));
}

void ModelList::choosefile(const QModelIndex &index)
{
    choosenote->setText("已选择：");
    QStandardItem *item = model->item(index.row(),0);
    filename->setText(item->text());
    confirmBtn->setEnabled(true);
}
