#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "budget.h"
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDoubleValidator>

int sort;
MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(B.afficher(sort));
    ui->lineEdit_id->setValidator(new QIntValidator(1,99999999,this));
    ui->lineEdit_id_2->setValidator(new QIntValidator(1,99999999,this));
    ui->lineEdit_id_3->setValidator(new QIntValidator(1,99999999,this));
    ui->lineEdit_pe->setValidator(new QDoubleValidator(0.0,99999.99,2,this));
    ui->lineEdit_pe_2->setValidator(new QDoubleValidator(0.0,99999.99,2,this));
    ui->lineEdit_pr->setValidator(new QDoubleValidator(0.0,99999.99,2,this));
    ui->lineEdit_pr_2->setValidator(new QDoubleValidator(0.0,99999.99,2,this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//add
void MainWindow::on_pushButton_7_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    float prix_e = ui->lineEdit_pe->text().toFloat();
    float prix_r = ui->lineEdit_pr->text().toFloat();
    Budget B(id, prix_e, prix_r);
    bool test = B.ajouter();
    QMessageBox msgBox;

    if (test) {
        msgBox.setText("Ajout avec succès");
        ui->tableView->setModel(B.afficher(sort));
    } else {
        msgBox.setText("Failed");
    }

    msgBox.exec();
}

//delete
void MainWindow::on_pushButton_8_clicked()
{
    Budget B1; B1.setid(ui->lineEdit_id_2->text().toInt());
    bool test=B1.supprimer(B1.getid());
    QMessageBox msgBox;
    if(test){
        msgBox.setText("Suppression avec succes");
        ui->tableView->setModel(B.afficher(sort));
    }
    else
        msgBox.setText("failed");
        msgBox.exec();
}
//update
void MainWindow::on_pushButton_9_clicked()
{
    int id = ui->lineEdit_id_3->text().toInt();
    float prix_e = ui->lineEdit_pe_2->text().toFloat();
    float prix_r = ui->lineEdit_pr_2->text().toFloat();
    Budget B(id, prix_e, prix_r);
    bool test = B.modifier();
    QMessageBox msgBox;

    if (test) {
        msgBox.setText("Update avec succès");
        ui->tableView->setModel(B.afficher(sort));
    } else {
        msgBox.setText("Failed");
    }

    msgBox.exec();
}

void MainWindow::on_pushButton_10_clicked()
{
    if(sort==1){
        sort=0;
    }else{
        sort=1;
    }
    ui->tableView->setModel(B.afficher(sort));
}

void MainWindow::on_pushButton_11_clicked()
{
    if(sort==2){
        sort=3;
    }else{
        sort=2;
    }
    ui->tableView->setModel(B.afficher(sort));
}

void MainWindow::on_pushButton_12_clicked()
{
    if(sort==4){
        sort=5;
    }else{
        sort=4;
    }
    ui->tableView->setModel(B.afficher(sort));
}

void MainWindow::on_pushButton_13_clicked()
{
    if(sort==6){
        sort=7;
    }else{
        sort=6;
    }
    ui->tableView->setModel(B.afficher(sort));
}
