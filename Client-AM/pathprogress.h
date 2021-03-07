#ifndef PATHPROGRESS_H
#define PATHPROGRESS_H

#include <QDialog>
#include <QProgressBar>
#include <QGridLayout>
#include <QTextEdit>
#include <QString>
class Pathprogress : public QDialog
{
    Q_OBJECT
public:
    explicit Pathprogress(QDialog *parent = nullptr);

private:
    QProgressBar *progressBar;
    QGridLayout *mainLayout;
    QTextEdit *outputLog;

public slots:
    void initProgress(int total);
    void updateProgress(int layer);
    void log(QString msg);
};

#endif // PATHPROGRESS_H
