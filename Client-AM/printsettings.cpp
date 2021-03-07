#include "printsettings.h"

Printsettings::Printsettings(QDialog *parent) : QDialog(parent)
{
    setWindowTitle("打印机设置");
    setFixedSize(250,150);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    nozzleSizeLabel = new QLabel("喷嘴大小：");
    nozzleSizeLabel->setFixedSize(50,25);
    nozzleSizeLineExit = new QLineEdit;
    nozzleSizeLineExit->setFixedSize(100,25);
    nozzleSizeLineExit->setText(nozzleSize);
    layerThicknessLabel = new QLabel("层厚：");
    layerThicknessLabel->setFixedSize(50,25);
    layerThicknessLineExit = new QLineEdit;
    layerThicknessLineExit->setFixedSize(100,25);
    layerThicknessLineExit->setText(layerThickness);
    confirmBtn = new QPushButton("确定");
    confirmBtn->setFixedSize(50,25);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(nozzleSizeLabel, 0,0);
    mainLayout->addWidget(nozzleSizeLineExit,0,1);
    mainLayout ->addWidget(layerThicknessLabel,1,0);
    mainLayout->addWidget(layerThicknessLineExit,1,1);
    mainLayout->addWidget(confirmBtn,2,2);
    mainLayout->setMargin(5);
    mainLayout->setSpacing(5);
    connect(confirmBtn,&QPushButton::pressed, this, &Printsettings::confirm);
    connect(confirmBtn,&QPushButton::pressed,this,&QDialog::close);
}


void Printsettings::confirm()
{
    nozzleSize = nozzleSizeLineExit->text();
    layerThickness = layerThicknessLineExit->text();
}


QString Printsettings::getPrintSettings()
{
    return " -s nozzleSize=" + nozzleSize +
           " -s layerThickness=" + layerThickness + " ";
}
