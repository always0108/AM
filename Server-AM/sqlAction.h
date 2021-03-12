#ifndef SQL_H
#define SQL_H

#include <qdebug.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QApplication>
#include <QDateTime>
#include <QList>
#include "myfile.h"

class SqlAction
{
public:
    SqlAction();
    bool init();
    void insert(QString filename);
    QList<MyFile> getFilesList(int pageSize,int pageNums);
    int getFilesNum();

private:
    QSqlDatabase db;
    QSqlQuery query;
    void createTable();
    void update(QString filename);
    bool checkFileExist(QString filename);
    QString getCurrentTime();
};

#endif // SQL_H
