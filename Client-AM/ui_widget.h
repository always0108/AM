/********************************************************************************
** Form generated from reading UI file 'showmodel.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_8;
    QLineEdit *lineEdit_file;
    QLabel *label_log;
    QVBoxLayout *verticalLayout_model;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QSlider *horizontalSlider_X;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QSlider *horizontalSlider_Y;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QSlider *horizontalSlider_Z;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(600, 600);
        verticalLayout = new QVBoxLayout(Widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(15);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_8 = new QLabel(Widget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_5->addWidget(label_8);

        lineEdit_file = new QLineEdit(Widget);
        lineEdit_file->setObjectName(QString::fromUtf8("lineEdit_file"));

        horizontalLayout_5->addWidget(lineEdit_file);

        label_log = new QLabel(Widget);
        label_log->setObjectName(QString::fromUtf8("label_log"));

        horizontalLayout_5->addWidget(label_log);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalLayout_model = new QVBoxLayout();
        verticalLayout_model->setObjectName(QString::fromUtf8("verticalLayout_model"));

        verticalLayout->addLayout(verticalLayout_model);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        horizontalSlider_X = new QSlider(Widget);
        horizontalSlider_X->setObjectName(QString::fromUtf8("horizontalSlider_X"));
        horizontalSlider_X->setMinimum(-99);
        horizontalSlider_X->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider_X);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        horizontalSlider_Y = new QSlider(Widget);
        horizontalSlider_Y->setObjectName(QString::fromUtf8("horizontalSlider_Y"));
        horizontalSlider_Y->setMinimum(-99);
        horizontalSlider_Y->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(horizontalSlider_Y);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_3->addWidget(label_4);

        horizontalSlider_Z = new QSlider(Widget);
        horizontalSlider_Z->setObjectName(QString::fromUtf8("horizontalSlider_Z"));
        horizontalSlider_Z->setMinimum(-1000);
        horizontalSlider_Z->setMaximum(1000);
        horizontalSlider_Z->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(horizontalSlider_Z);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "StlLoader", nullptr));
        label->setText(QApplication::translate("Widget", "Load STL model Test", nullptr));
        label_8->setText(QApplication::translate("Widget", "select STL mode:", nullptr));
        lineEdit_file->setPlaceholderText(QApplication::translate("Widget", "please enter to choose a STL file", nullptr));
        label_log->setText(QString());
        label_2->setText(QApplication::translate("Widget", "eye X", nullptr));
        label_3->setText(QApplication::translate("Widget", "eye Y", nullptr));
        label_4->setText(QApplication::translate("Widget", "eye Z", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
