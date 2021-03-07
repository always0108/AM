#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QString>

class Serversettings : public QDialog
{
    Q_OBJECT
public:
    explicit Serversettings(QDialog *parent = nullptr);
    QString ip = "127.0.0.1";
    int port = 8010;

signals:

private slots:
    void confirmIp();
private:
    QLabel *ipLabel;
    QLabel *portLabel;
    QLineEdit *ipLineExit;
    QLineEdit *portLineExit;
    QGridLayout *mainLayout;
    QPushButton *confirmBtn;
};

#endif // SETTINGS_H
