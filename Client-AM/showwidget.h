#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QOpenGLWidget>
#include <QtCharts>
#include "showpath.h"
#include "showmodel.h"

QT_CHARTS_USE_NAMESPACE
class ShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowWidget(QWidget *parent = nullptr);
    QLabel *output;
    QTextEdit *textEdit_Recv;
    Showpath *showpath;
    QChartView* chartViewtmp;
    showmodel* showModel;

signals:

};

#endif // SHOWWIDGET_H
