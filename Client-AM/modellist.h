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
    QGridLayout *mainLayout;
    QHBoxLayout *noteLayout;
private slots:
    void choosefile(const QModelIndex &index);
    void onClicked();
public slots:
    void initModelList(int size);
    QString getTargetfile();

};

#endif // MODELLIST_H
