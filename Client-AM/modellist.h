#ifndef MODELLIST_H
#define MODELLIST_H

#include <QDialog>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QScrollBar>
#include <QtMath>
#include <QMessageBox>
#include "tcpclient.h"

class ModelList :public QDialog
{
    Q_OBJECT
public:
    explicit ModelList(QDialog *parent = nullptr);

private:
    QLabel *choosenote;
    QLabel *filename;
    QTableView *tableView;
    QStandardItemModel *model;
    QPushButton *confirmBtn;
    QLabel *current;
    QLabel *space;
    QLabel *total;
    QPushButton *prePage;
    QPushButton *nextPage;
    QGridLayout *mainLayout;
    QHBoxLayout *noteLayout;
    QHBoxLayout *pageLayout;
    int count;
    int pageSize;
    int totalPage;
    int currentPage;
    Tcpclient *tcpclient;
private slots:
    void choosefile(const QModelIndex &index);
    void onClicked();
    void pre();
    void next();
    void update();
    void sendSignal(QString msg);
    void showMessageBox(QString msg);

public slots:
    void initModelList(int fileNum,int pageSize,Tcpclient *tcpclient);
    void updateData(int size , QString list);
};

#endif // MODELLIST_H
