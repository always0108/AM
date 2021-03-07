#ifndef PRINTSETTINGS_H
#define PRINTSETTINGS_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>

class Printsettings : public QDialog
{
    Q_OBJECT
public:
    explicit Printsettings(QDialog *parent = nullptr);
    QString nozzleSize = "0.4";         //喷嘴大小
    QString layerThickness = "0.2";     //层厚
    int initialLayerThickness;//初始层厚
    int layer0extrusionWidth;//初始层线宽
    int extrusionWidth; //挤出线宽
    int insetCount;
    int downSkinCount;
    int upSkinCount;
    int skirtDistance;
    int skirtLineCount;
    int skirtMinLength;

    //Retraction settings

    int initialSpeedupLayers;
    int initialLayerSpeed;
    int printSpeed; //打印速度
    int inset0Speed;
    int insetXSpeed;
    int moveSpeed;
    int fanFullOnLayerNr;

    //Infill settings
    int sparseInfillLineDistance;
    int infillOverlap;//填充料交叠
    int infillSpeed;  //填充速度
    int infillPattern;//填充类型
    int skinSpeed;
    int perimeterBeforeInfill;

    //Support material
    int supportType; //支撑类型
    int supportAngle;
    int supportEverywhere;
    int supportLineDistance;
    int supportXYDistance;
    int supportZDistance;
    int supportExtruder;
    QString getPrintSettings();

signals:

private slots:
    void confirm();

private:
    QLabel *nozzleSizeLabel;
    QLabel *layerThicknessLabel;
    QLineEdit *nozzleSizeLineExit;
    QLineEdit *layerThicknessLineExit;
    QGridLayout *mainLayout;
    QPushButton *confirmBtn;
};

#endif // PRINTSETTINGS_H
