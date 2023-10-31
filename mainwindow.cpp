#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "forniseur.h"
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDoubleValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(F.afficher());
    ui->le_id->setValidator(new QIntValidator(1,99999999,this));
    ui->le_id_2->setValidator(new QIntValidator(1,99999999,this));
    ui->le_id_3->setValidator(new QIntValidator(1,99999999,this));
    ui->le_tel->setValidator(new QIntValidator(10000000,99999999,this));
    ui->le_tel_2->setValidator(new QIntValidator(10000000,99999999,this));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ajouter_clicked()
{
    int id = ui->le_id->text().toInt();
    QString nom = ui->le_nom->text();
    QString email = ui->le_email->text();
    int tel = ui->le_tel->text().toInt();
    QString adresse = ui->le_adresse->text();

    Forniseur F(id, nom, email,tel,adresse);
    bool test = F.ajouter();
    QMessageBox msgBox;

    if (test) {
        msgBox.setText("Ajout avec succès");
        ui->tableView->setModel(F.afficher());
    } else {
        msgBox.setText("Failed");
    }

    msgBox.exec();
}

void MainWindow::on_supprimer_clicked()
{
    Forniseur F1; F1.setid(ui->le_id_3->text().toInt());
    bool test=F1.supprimer(F1.getid());
    QMessageBox msgBox;
    if(test){
        msgBox.setText("Suppression avec succes");
        ui->tableView->setModel(F.afficher());
    }
    else
        msgBox.setText("failed");
        msgBox.exec();
}


void MainWindow::on_Modifier_clicked()
{
    int id = ui->le_id_2->text().toInt();
    QString nom = ui->le_nom_2->text();
    QString email = ui->le_email_2->text();
    int tel = ui->le_tel_2->text().toInt();
    QString adresse = ui->le_adresse_2->text();

    Forniseur F(id, nom, email,tel,adresse);
    bool test = F.modifier();
    QMessageBox msgBox;

    if (test) {
        msgBox.setText("Update avec succès");
        ui->tableView->setModel(F.afficher());
    } else {
        msgBox.setText("Failed");
    }

    msgBox.exec();
}
