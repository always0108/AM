#include "SqlAction.h"

SqlAction::SqlAction()
{

}

bool SqlAction::init()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QApplication::applicationDirPath()+"/file.dat");    //如果本目录下没有该文件,则会在本目录下生成,否则连接该文件
    if (!db.open()){
        qDebug() << "Error:  Fail to connect database!";
        return false;
    }
    qDebug() << "Connect database succeessfully!";
    query = QSqlQuery(db);
    createTable();
    return true;
}

void SqlAction::createTable()
{
    if(!query.exec("CREATE TABLE IF NOT EXISTS file(id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(255) NOT NULL, time VARCHAR(20) NOT NULL);"))
    {
        qDebug() << "Error: Fail to create table !";
    }
}

void SqlAction::insert(QString filename)
{

    if(checkFileExist(filename)){
        query.exec("INSERT INTO file (name, time) VALUES ('" + filename + "','" + getCurrentTime() + "');");
    }else{
        update(filename);
    }
}

QList<MyFile> SqlAction::getFilesList()
{
    query.exec("SELECT name,time FROM file ORDER BY name;");
    QList<MyFile> lists;
    while(query.next())
        lists.push_back(MyFile(query.value(0).toString(),query.value(1).toString()));
    return lists;
}

void SqlAction::update(QString filename)
{
    query.exec("UPDATE file SET time = '" + getCurrentTime() + "' WHERE name = '" + filename + "';");
}


bool SqlAction::checkFileExist(QString filename)
{
    query.exec("SELECT * FROM file WHERE name == '"+ filename + "';");
    if(query.next()){
        return false;
    }else{
        return true;
    }
}

QString SqlAction::getCurrentTime()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    return current_date_time.toString("yyyy-MM-dd hh:mm:ss");
}


