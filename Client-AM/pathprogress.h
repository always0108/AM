#ifndef PATHPROGRESS_H
#define PATHPROGRESS_H

#include <QDialog>
#include <QProgressBar>
#include <QGridLayout>
#include <QTextEdit>
#include <QString>
#include <QMessageBox>
#include <QCloseEvent>
class Pathprogress : public QDialog
{
    Q_OBJECT
public:
    explicit Pathprogress(QDialog *parent = nullptr);

private:
    QProgressBar *progressBar;
    QGridLayout *mainLayout;
    QTextEdit *outputLog;
    bool status = false;

public slots:
    void initProgress(int total);
    void updateProgress(int layer);
    void log(QString msg);
    bool getStatus();
    void setStatus(bool status);
    void closeEvent(QCloseEvent *event);
};

#endif // PATHPROGRESS_H
