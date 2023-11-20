// logindialog.h
#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_loginButton_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGIN_DIALOG_H
