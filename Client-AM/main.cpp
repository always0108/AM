#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow* w = new MainWindow();
    w->resize(1000,800);
    w->show();
    return a.exec();
}
