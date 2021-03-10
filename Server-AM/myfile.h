#ifndef MYFILE_H
#define MYFILE_H

#include<QString>

class MyFile
{
public:
    MyFile();
    MyFile(QString name,QString time);
    QString getName();
    void setName(QString name);
    QString getTime();
    void setTime(QString time);

private:
    QString name;
    QString time;
};

#endif // MYFILE_H
