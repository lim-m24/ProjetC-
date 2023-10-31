#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(afficherClients());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int id = ui->lineEdit->text().toInt();
    QString nom = ui->lineEdit_2->text();
    QString tel = ui->lineEdit_4->text();
    int modePaiement = ui->comboBox->currentIndex();
    double montantDepense = ui->lineEdit_3->text().toDouble();

    Client c(nom, tel, id, modePaiement, montantDepense);
    if (c.ajouter()) {
        ui->tableView->setModel(afficherClients());
        QMessageBox::information(nullptr, QObject::tr("Operation Successful"),
                    QObject::tr("SUCCESS.\n"), QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlQuery query;
    QString id = ui->lineEdit_5->text();
    query.prepare("DELETE from table_des_clients where ID= :id");
    query.bindValue(":id", id);
    if (query.exec()) {
        ui->tableView->setModel(afficherClients());
        QMessageBox::information(nullptr, QObject::tr("Operation Successful"),
                    QObject::tr("SUCCESS.\n"), QMessageBox::Ok);
    }
    else {
        QMessageBox::critical(nullptr, QObject::tr("Operation Failed"),
                    QObject::tr("ERROR.\n"), QMessageBox::Cancel);
    }
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{

}

void MainWindow::on_label_objectNameChanged(const QString &objectName)
{

}
