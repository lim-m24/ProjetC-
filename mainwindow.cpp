#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include <QMessageBox>
#include <iostream>
#include <QDebug>

int sort;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    sort=0;
    ui->setupUi(this);
    int ret=A.connect_arduino();
        switch(ret){
        case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
            break;
        case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
           break;
        case(-1):qDebug() << "arduino is not available";
        }
         QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));
    ui->lineEdit->setValidator(new QIntValidator(0,99999,this));
    //ui->lineEdit_3->setValidator(new QDoubleValidator(0.0, 99999.0, 2, this));
    ui->lineEdit_4->setValidator(new QIntValidator(0,99999999,this));
    ui->tableView->setModel(c.afficherClients(sort));
    ui->tableView_2->setModel(c.afficherHistoriqueAchats());
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
        ui->tableView->setModel(c.afficherClients(sort));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlQuery query;
    QString id = ui->lineEdit_5->text();
    query.prepare("DELETE from table_des_clients where ID= :id");
    query.bindValue(":id", id);
    if (query.exec()) {
        ui->tableView->setModel(c.afficherClients(sort));
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

void MainWindow::on_pushButton_3_clicked()
{
    int id = ui->lineEdit->text().toInt();
    QString nom = ui->lineEdit_2->text();
    QString tel = ui->lineEdit_4->text();
    int modePaiement = ui->comboBox->currentIndex();
    double montantDepense = ui->lineEdit_3->text().toDouble();

    Client c(nom, tel, id, modePaiement, montantDepense);
    if (c.modifier()) {
        ui->tableView->setModel(c.afficherClients(sort));
        QMessageBox::information(nullptr, QObject::tr("Operation Successful"),
                    QObject::tr("SUCCESS.\n"), QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QString text=ui->comboBox_2->currentText();
    if(text=="montant depensé"){
        sort=1;
    }else if (text == "id") {
                sort = 2;
            } else if (text == "nom") {
                sort = 3;
            }
        ui->tableView->setModel(c.afficherClients(sort));

}

void MainWindow::on_lineEdit_6_textChanged(const QString &search)
{
    if(search.isEmpty()){
        ui->tableView->setModel(c.afficherClients(sort));
    }else{
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM table_des_clients WHERE NOMCLIENT LIKE '"+ search + "%' OR NUMCLIENT LIKE '"+ search + "%' OR ID LIKE '"+ search + "%' OR MONTANTDEPENSE LIKE '"+ search + "%' " );
        ui->tableView->setModel(model);
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    s = new stat_combo();
            s->setWindowTitle("statistique disponibilité");
              s->choix_pie();
              s->show();
}

void MainWindow::on_pushButton_6_clicked()
{
    QString idToExport = ui->lineEdit_7->text();
               Client C;
               QString details = c.getDetailsByID(idToExport);

               if (!details.isEmpty()) {
                   QString fileName = "client_details_" + idToExport + ".pdf";

                   QFile file(fileName);
                   if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
                       QTextStream stream(&file);
                       stream << details;
                       file.close();

                       QMessageBox::information(nullptr, QObject::tr("Export Successful"),
                                                QObject::tr("Details exported to %1").arg(fileName),
                                                QMessageBox::Ok);
                   } else {
                       qDebug() << "Error: Unable to create or write to the PDF file.";
                       QMessageBox::critical(nullptr, QObject::tr("Export Error"),
                                             QObject::tr("Unable to create or write to the PDF file."),
                                             QMessageBox::Ok);
                   }
               } else {
                   qDebug() << "Error: Employee not found or an error occurred.";
                   QMessageBox::critical(nullptr, QObject::tr("Export Error"),
                                         QObject::tr("Employee not found or an error occurred."),
                                         QMessageBox::Ok);
       }
}
void MainWindow::update_label()
{
    data=A.read_from_arduino();

    if(data=="1")

        ui->label_3->setText("ON"); // si les données reçues de arduino via la liaison série sont égales à 1
    // alors afficher ON

    else if (data=="0")

        ui->label_3->setText("OFF");   // si les données reçues de arduino via la liaison série sont égales à 0
     //alors afficher ON
}
