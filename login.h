#ifndef LOGIN_H
#define LOGIN_H

#include"mainwindow.h"
#include <QMainWindow>

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void handleRFIDScan(QByteArray data);

private slots:
    void on_pushButton_Login_clicked();
    void readData();

private:
    Ui::Login *ui;
    MainWindow *mainwindow;
    QByteArray buffer;
    MainWindow *mainwindowInstance;
};

#endif // LOGIN_H
