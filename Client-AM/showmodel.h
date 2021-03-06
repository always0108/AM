#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "glwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class showmodel : public QWidget
{
    Q_OBJECT

public:
    explicit showmodel(QWidget *parent = nullptr);
    ~showmodel();

private slots:
    void slot_choose_model();

private:
    Ui::Widget *ui;
    GLWindow *m_glWin = nullptr;
};
#endif // WIDGET_H
