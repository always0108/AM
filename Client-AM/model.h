#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QList>
#include <QFile>
#include <QMessageBox>
#include <QVector3D>

typedef struct modelSize{
    float smallX;
    float largeX;
    float smallY;
    float largeY;
    float smallZ;
    float largeZ;
} modelSize;

class STLModel : public QObject
{
    Q_OBJECT
public:
    explicit STLModel(QObject *parent = 0);
public:
    modelSize size;
    QVector<float> vPoints;
public:
    void model_load(QString path);
    void model_readText(QString path);
    void model_readBinary(QString path);
    void model_clear();

    modelSize model_getSize();
    void model_getSizeWhileReadPoint(QVector3D point);
    QVector3D model_getCenter(modelSize size);
private:
    QVector3D getCoordinateFromString(QString line);
};

#endif // MODEL_H
