#include "showwidget.h"
#include <QGridLayout>
#include <QVBoxLayout>
ShowWidget::ShowWidget(QWidget *parent) : QWidget(parent)
{
    output = new QLabel("状态输出窗口");
    output->resize(150,50);

    textEdit_Recv = new QTextEdit;
    textEdit_Recv->setFixedHeight(100);
    showModel = new showmodel(this);
    showpath = new Showpath(this);
    showpath->qwtPlot->resize(420,700);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(10);
    rightLayout->addWidget(showpath->qwtPlot);
    rightLayout->addWidget(output);
    rightLayout->addWidget(textEdit_Recv);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(showModel);
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setMargin(10);
    mainLayout->setSpacing(10);
    mainLayout->addLayout(leftLayout,0,0);
    mainLayout->addLayout(rightLayout,0,1);
    mainLayout->setColumnStretch(0,1);
    mainLayout->setColumnStretch(1,1);
}
