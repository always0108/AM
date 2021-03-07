#include "serversettings.h"
#include <iostream>
Serversettings::Serversettings(QDialog *parent) : QDialog(parent)
{
    setWindowTitle("服务器参数");
    setFixedSize(250,150);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    ipLabel = new QLabel("IP地址: ");
    ipLabel->setFixedSize(50,25);
    ipLineExit = new QLineEdit;
    ipLineExit->setFixedSize(100,25);
    ipLineExit->setText("127.0.0.1");
    portLabel = new QLabel("端口: ");
    portLabel->setFixedSize(50,25);
    portLineExit = new QLineEdit;
    portLineExit->setFixedSize(100,25);
    portLineExit->setText("8010");
    confirmBtn = new QPushButton("确定");
    confirmBtn->setFixedSize(50,25);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(ipLabel, 0,0);
    mainLayout->addWidget(ipLineExit,0,1);
    mainLayout ->addWidget(portLabel,1,0);
    mainLayout->addWidget(portLineExit,1,1);
    mainLayout->addWidget(confirmBtn,2,2);
    mainLayout->setMargin(5);
    mainLayout->setSpacing(5);
    connect(confirmBtn,&QPushButton::pressed, this, &Serversettings::confirmIp);
    connect(confirmBtn,&QPushButton::pressed,this,&QDialog::close);

}

void Serversettings::confirmIp()
{
    ip = ipLineExit->text();
    port = portLineExit->text().toInt();
}
