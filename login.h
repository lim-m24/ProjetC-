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

private slots:
    void on_pushButton_Login_clicked();

private:
    Ui::Login *ui;
    MainWindow *mainwindow;
};

#endif // LOGIN_H
