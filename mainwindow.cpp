#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employe.h"
#include <QMessageBox>
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QObject>

#include <QValidator>
#include <QLineEdit>
#include <QRegularExpression>


int sort;
int form=0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    sort=0;
    ui->setupUi(this);
    ui->tableView->setModel(E.afficher(sort));
    ui->le_id->setValidator(new QIntValidator(0,9999999,this));
    ui->le_id_2->setValidator(new QIntValidator(0,9999999,this));
    ui->le_id_3->setValidator(new QIntValidator(0,9999999,this));

    QRegularExpression uppercase("[A-Z]");
    QRegularExpression digit("[0-9]");
    ui->le_password->setEchoMode(QLineEdit::Password);

    QObject::connect(ui->le_password, &QLineEdit::textChanged, [=]() {
        QString password = ui->le_password->text();

        if (password.length() < 3 ||
            !password.contains(uppercase) ||
            !password.contains(digit)) {
            ui->le_password->setStyleSheet("QLineEdit { border: 1px solid red; }");
            ui->le_password->setToolTip("Password must contain at least one uppercase letter, one digit, and be at least 3 characters long.");
        } else {
            ui->le_password->setStyleSheet(""); // Reset style
            ui->le_password->setToolTip("");
        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int id_E = ui->le_id->text().toInt();
    QString nom = ui->le_nom->text();
    QString prenom = ui->le_prenom->text();
    QString email = ui->le_mail->text();
    QString username = ui->le_username->text();
    QString password = ui->le_password->text();
    QString role = ui->comboBox->currentText();

    QMessageBox msgBox;
    Employe E( id_E ,nom,prenom,email,role,username,password);
    if (E.ajouter()) {
        ui->tableView->setModel(E.afficher(sort));
        /*QMessageBox::information(nullptr, QObject::tr("Operation Successful"),
                    QObject::tr("SUCCESS.\n"), QMessageBox::Ok);*/
        //msgBox.setText("Ajout avec succès");
    }else{
        QMessageBox::critical(nullptr, QObject::tr("Operation Failed"),
                    QObject::tr("ERROR.\n"), QMessageBox::Cancel);
    }
    //msgBox.exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlQuery query;
    int id_E = ui->le_id_2->text().toInt();
    query.prepare("DELETE from EMPLOYE where ID_E=:id");
    query.bindValue(":id", id_E);
    if (query.exec()) {
        ui->tableView->setModel(E.afficher(sort));
        QMessageBox::information(nullptr, QObject::tr("Operation Successful"),
                    QObject::tr("SUCCESS.\n"), QMessageBox::Ok);
    }
    else {
        QMessageBox::critical(nullptr, QObject::tr("Operation Failed"),
                    QObject::tr("ERROR.\n"), QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    int id_E = ui->le_id_3->text().toInt();
    QString nom = ui->le_nom_2->text();
    QString prenom = ui->le_prenom_2->text();
    QString email = ui->le_email_2->text();
    QString role = ui->comboBox_2->currentText();


    Employe E( id_E ,nom,prenom,email,role);
    if (E.Modifier()) {
        ui->tableView->setModel(E.afficher(sort));
        QMessageBox::information(nullptr, QObject::tr("Operation Successful"),
                    QObject::tr("SUCCESS.\n"), QMessageBox::Ok);
    }else{
        QMessageBox::critical(nullptr, QObject::tr("Operation Failed"),
                    QObject::tr("ERROR.\n"), QMessageBox::Cancel);
    }
}

void MainWindow::on_upmail_clicked()
{
    Employe E;

       QString mail = ui->le_mail->text();

       if (E.valideremail(mail))

          {

              QMessageBox::information(this, "Validation", "Adresse e-mail valide.");

          }

          else

          {

              QMessageBox::warning(this, "Validation", "Adresse e-mail non valide.");

          }
}

void MainWindow::on_pushButton_4_clicked()
{
    QString tr=ui->comboBox_3->currentText();
    if(tr=="id"){
        if(sort==0){
            sort=1;
        }else{
            sort=0;
        }
    }
    if(tr=="nom"){
        if(sort==3){
            sort=2;
        }else{
            sort=3;
        }
    }


    ui->tableView->setModel(E.afficher(sort));
}


void MainWindow::on_lineEdit_textChanged(const QString &text)
{
    if(text.isEmpty()){
        ui->tableView->setModel(E.afficher(sort));
    }else{
        QSqlQueryModel * model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM EMPLOYE WHERE ID_E LIKE '" + text +"%' OR NOM LIKE '" + text +"%' OR PRENOM LIKE '" + text +"%' OR EMAIL LIKE '" + text +"%'");
        ui->tableView->setModel(model);
    }
}




void MainWindow::on_pushButton_5_clicked()
{
    QString idToExport = ui->lineEdit_2->text();
            Employe E;
            QString details = E.getDetailsByID(idToExport);

            if (!details.isEmpty()) {
                QString fileName = "employee_details_" + idToExport + ".pdf";

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

void MainWindow::on_eye_clicked()
{

    if(form==0){
        ui->le_password->setEchoMode(QLineEdit::Normal);
        form=1;
    }else{
        ui->le_password->setEchoMode(QLineEdit::Password);
        form=0;
    }
}
