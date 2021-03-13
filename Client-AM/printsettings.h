#ifndef PRINTSETTINGS_H
#define PRINTSETTINGS_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QRegExpValidator>

class Printsettings : public QDialog
{
    Q_OBJECT
public:
    explicit Printsettings(QDialog *parent = nullptr);

    QString getPrintSettings();

signals:

private slots:
    void confirm();

private:
    //激光功率：
    QString laserPower;
    //扫描速度：
    QString scanSpeed;
    //层厚：
    QString layerThickness;
    //扫描间距：
    QString scanPitch;
    //光斑大小：
    QString spotSize;

    QLabel *laserPowerLabel;
    QLabel *scanSpeedLabel;
    QLabel *layerThicknessLabel;
    QLabel *scanPitchLabel;
    QLabel *spotSizeLabel;

    QLineEdit *laserPowerLineExit;
    QLineEdit *scanSpeedLineExit;
    QLineEdit *layerThicknessLineExit;
    QLineEdit *scanPitchLineExit;
    QLineEdit *spotSizeLineExit;

    QLabel *laserPowerUnitLabel;
    QLabel *scanSpeedUnitLabel;
    QLabel *layerThicknessUnitLabel;
    QLabel *scanPitchUnitLabel;
    QLabel *spotSizeUnitLabel;

    QGridLayout *mainLayout;
    QPushButton *confirmBtn;
};

#endif // PRINTSETTINGS_H
