#include "showmodel.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QFileDialog>
#include "model.h"


showmodel::showmodel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->lineEdit_file, SIGNAL(returnPressed()), this, SLOT(slot_choose_model()));
}

showmodel::~showmodel()
{
    delete ui;
}

void showmodel::slot_choose_model()
{
    QString file_name = QFileDialog::getOpenFileName(nullptr, "open", ".", "*.stl");
    ui->lineEdit_file->setText(file_name);

    if (file_name.isEmpty())
    {
        ui->label_log->setText("File is not existed!");
        return;
    }

    STLModel* stlLoader = new STLModel();
    stlLoader->model_load(file_name);

    if(stlLoader->vPoints.size() < 3)
    {
        return;
    }

    m_glWin = new GLWindow(stlLoader);
    ui->verticalLayout_model->addWidget(m_glWin);
    connect(ui->horizontalSlider_X, SIGNAL(valueChanged(int)), m_glWin, SLOT(slot_cam_X(int)));
    connect(ui->horizontalSlider_Y, SIGNAL(valueChanged(int)), m_glWin, SLOT(slot_cam_Y(int)));
    connect(ui->horizontalSlider_Z, SIGNAL(valueChanged(int)), m_glWin, SLOT(slot_cam_Z(int)));
}
