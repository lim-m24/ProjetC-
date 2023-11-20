#include "login.h"
#include "ui_login.h"
#include<QMessageBox>
Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_Login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();


    QSqlQuery query,query2;
    query.prepare("SELECT ID_E FROM EMPLOYE WHERE USERNAME = :username AND PASSWORD = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (query.exec() && query.next()) {
        QString idd=query.value(0).toString();
        query2.prepare("SELECT NOM,PRENOM FROM EMPLOYE WHERE ID_E = :idd");
        query2.bindValue(":idd", idd);
        if(query2.exec() &&query2.next()){
            QString nom=query2.value(0).toString();
            QString prenom=query2.value(1).toString();
            QMessageBox::information(this,"Login","welcome "+nom+" "+prenom);
        }
        hide();
        mainwindow = new MainWindow(this);
        mainwindow->show();
    }else {
    QMessageBox::warning(this,"Login","Username and password is not correct");
    }
}
