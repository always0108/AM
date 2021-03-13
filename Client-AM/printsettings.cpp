#include "printsettings.h"

Printsettings::Printsettings(QDialog *parent) : QDialog(parent)
{
    setWindowTitle("打印机设置");
    setFixedSize(300,300);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);

    laserPower = "110";
    scanSpeed = "200";
    layerThickness = "50";
    scanPitch = "50";
    spotSize = "60";

    // 创建提示文本
    laserPowerLabel = new QLabel("激光功率：");
    laserPowerLabel->setFixedSize(60,25);
    scanSpeedLabel = new QLabel("扫描速度：");
    scanSpeedLabel->setFixedSize(60,25);
    layerThicknessLabel = new QLabel("层厚：");
    layerThicknessLabel->setFixedSize(60,25);
    scanPitchLabel = new QLabel("扫描间距：");
    scanPitchLabel->setFixedSize(60,25);
    spotSizeLabel = new QLabel("光斑大小：");
    spotSizeLabel->setFixedSize(60,25);

    // 设置的输入只能是数字
    QRegExp regExp("[0-9]{1,10}");
    laserPowerLineExit = new QLineEdit;
    laserPowerLineExit->setValidator(new QRegExpValidator(regExp, this));
    laserPowerLineExit->setText(laserPower);
    laserPowerLineExit->setFixedSize(120,25);
    scanSpeedLineExit = new QLineEdit;
    scanSpeedLineExit->setValidator(new QRegExpValidator(regExp, this));
    scanSpeedLineExit->setText(scanSpeed);
    scanSpeedLineExit->setFixedSize(120,25);
    layerThicknessLineExit = new QLineEdit;
    layerThicknessLineExit->setValidator(new QRegExpValidator(regExp, this));
    layerThicknessLineExit->setText(layerThickness);
    layerThicknessLineExit->setFixedSize(120,25);
    scanPitchLineExit = new QLineEdit;
    scanPitchLineExit->setValidator(new QRegExpValidator(regExp, this));
    scanPitchLineExit->setText(scanPitch);
    scanPitchLineExit->setFixedSize(120,25);
    spotSizeLineExit = new QLineEdit;
    spotSizeLineExit->setValidator(new QRegExpValidator(regExp, this));
    spotSizeLineExit->setText(spotSize);
    spotSizeLineExit->setFixedSize(120,25);

    //创建单位文本
    laserPowerUnitLabel = new QLabel("(W)");
    laserPowerUnitLabel->setFixedSize(50,25);
    scanSpeedUnitLabel = new QLabel("(mm/s)");
    scanSpeedUnitLabel->setFixedSize(50,25);
    layerThicknessUnitLabel = new QLabel("µm");
    layerThicknessUnitLabel->setFixedSize(50,25);
    scanPitchUnitLabel = new QLabel("µm");
    scanPitchUnitLabel->setFixedSize(50,25);
    spotSizeUnitLabel = new QLabel("µm");
    spotSizeUnitLabel->setFixedSize(50,25);

    confirmBtn = new QPushButton("确定");
    confirmBtn->setFixedSize(50,25);

    mainLayout = new QGridLayout(this);
    mainLayout->setMargin(10);
    mainLayout->setSpacing(5);

    mainLayout->addWidget(laserPowerLabel,0,0);
    mainLayout->addWidget(scanSpeedLabel,1,0);
    mainLayout ->addWidget(layerThicknessLabel,2,0);
    mainLayout->addWidget(scanPitchLabel,3,0);
    mainLayout->addWidget(spotSizeLabel,4,0);

    mainLayout->addWidget(laserPowerLineExit,0,1);
    mainLayout->addWidget(scanSpeedLineExit,1,1);
    mainLayout ->addWidget(layerThicknessLineExit,2,1);
    mainLayout->addWidget(scanPitchLineExit,3,1);
    mainLayout->addWidget(spotSizeLineExit,4,1);

    mainLayout->addWidget(laserPowerUnitLabel,0,2);
    mainLayout->addWidget(scanSpeedUnitLabel,1,2);
    mainLayout ->addWidget(layerThicknessUnitLabel,2,2);
    mainLayout->addWidget(scanPitchUnitLabel,3,2);
    mainLayout->addWidget(spotSizeUnitLabel,4,2);

    mainLayout->addWidget(confirmBtn,5,2);

    connect(confirmBtn,&QPushButton::pressed, this, &Printsettings::confirm);
    connect(confirmBtn,&QPushButton::pressed,this,&QDialog::close);
}


void Printsettings::confirm()
{
    laserPower = laserPowerLineExit->text();
    scanSpeed = scanSpeedLineExit->text();
    layerThickness = layerThicknessLineExit->text();
    scanPitch = scanPitchLineExit->text();
    spotSize = spotSizeLineExit->text();
}


QString Printsettings::getPrintSettings()
{
    return "laserPower=" + laserPower + "|" +
           "scanSpeed=" + scanSpeed + "|" +
           "layerThickness=" + layerThickness + "|" +
           "scanPitch=" + scanPitch + "|" +
           "spotSize=" + spotSize;
}
