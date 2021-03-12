#include "pathprogress.h"

Pathprogress::Pathprogress(QDialog *parent) : QDialog(parent)
{
    setWindowTitle("填充进度");
    setFixedSize(300,200);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    progressBar = new QProgressBar(this);
    progressBar->setOrientation(Qt::Horizontal);  // 水平方向
    progressBar->setAlignment(Qt::AlignVCenter);
    outputLog = new QTextEdit;
    outputLog->setFixedHeight(120);
    outputLog->setReadOnly(true);
    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(progressBar, 0,0);
    mainLayout->addWidget(outputLog, 1,0);
    mainLayout->setMargin(5);
    mainLayout->setSpacing(5);
}

void Pathprogress::initProgress(int total)
{
    progressBar->setMinimum(0);     // 最小值
    progressBar->setMaximum(total); // 最大值
    progressBar->setValue(0);       //当前进度
    progressBar->setFormat(tr("Current progress : %1%").arg(QString::number(0, 'f', 1)));
    outputLog->clear();
    status = true;
}

void Pathprogress::updateProgress(int layer)
{
    if(layer != progressBar->value()){
        progressBar->setValue(layer);
        double dProgress = (progressBar->value() - progressBar->minimum()) * 100.0
                        / (progressBar->maximum() - progressBar->minimum()); // 百分比计算公式
        progressBar->setFormat(QString::fromLocal8Bit("当前进度为：%1%").arg(QString::number(dProgress, 'f', 1)));
        progressBar->setFormat(tr("Current progress : %1%").arg(QString::number(dProgress, 'f', 1)));
        if(layer == progressBar->maximum()){
            log("填充已完成 ! ");
        }
    }
}

void Pathprogress::log(QString msg)
{
    outputLog->append(msg);
    QTextCursor cursor = outputLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    outputLog->setTextCursor(cursor);
}

bool Pathprogress::getStatus()
{
    return status;
}

void Pathprogress::setStatus(bool status)
{
    this->status = status;
}

void Pathprogress::closeEvent(QCloseEvent *event)
{
    if(status){
        QMessageBox::warning(this, "警告", "路径填充未结束，请稍候" , QMessageBox::Ok , QMessageBox::Ok);
        event->ignore();
    }else{
        event->accept();
    }
}
