#include "myfile.h"

MyFile::MyFile()
{

}

MyFile::MyFile(QString name,QString time)
{
    this->name = name;
    this->time = time;
}

QString MyFile::getName()
{
    return name;
}

void MyFile::setName(QString name)
{
    this->name = name;
}

QString MyFile::getTime()
{
    return time;
}

void MyFile::setTime(QString time)
{
    this->time = time;
}


