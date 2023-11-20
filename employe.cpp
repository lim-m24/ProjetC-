#include "employe.h"
#include <QRegularExpression>
#include <iostream>
#include <QDebug>
#include<QPainter>
#include<QPrinter>
#include<QMessageBox>
Employe::Employe(int id_E_ ,QString nom_, QString prenom_, QString email_, QString role_,QString username_,QString password_)
{
    id_E=id_E_;
    nom=nom_;
    prenom = prenom_;
    email = email_;
    role = role_;
    username=username_;
    password=password_;

}

QSqlQueryModel* Employe::afficher(int sort)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    QString sortby;

    switch(sort){
    case 0:
        sortby="ID_E DESC";
        break;
    case 1:
        sortby="ID_E ASC";
        break;
    case 2:
        sortby="NOM DESC";
        break;
    case 3:
        sortby="NOM ASC";
        break;
    }
    model->setQuery("select * from EMPLOYE ORDER BY " +sortby );
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("role"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("username"));

    return model;
}

bool Employe::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYE (ID_E, NOM, PRENOM, EMAIL, ROLE, USERNAME, PASSWORD) VALUES (:ID_E, :NOM, :PRENOM, :EMAIL, :ROLE, :USERNAME, :PASSWORD);");

    QString id_E_ = QString::number(id_E);
    QString nom_ = QString(nom);
    QString prenom_ = QString(prenom);
    QString email_ = QString(email);
    QString role_ = QString(role);
    QString username_ = QString(username);
    QString password_ = QString(password);

    query.bindValue(":ID_E", id_E_);
    query.bindValue(":NOM", nom_);
    query.bindValue(":PRENOM", prenom_);
    query.bindValue(":EMAIL", email_);
    query.bindValue(":ROLE", role_);
    query.bindValue(":USERNAME", username_);
    query.bindValue(":PASSWORD", password_);

    return (query.exec());
}

bool Employe::Modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYE SET NOM=:nom,PRENOM=:prenom, EMAIL=:email,ROLE=:role WHERE ID_E=:id_E;");

    QString id_E_ = QString::number(id_E);
    QString nom_ = QString(nom);
    QString prenom_ = QString(prenom);
    QString email_ = QString(email);
    QString role_ = QString(role);

    query.bindValue(":id_E", id_E_);
    query.bindValue(":nom", nom_);
    query.bindValue(":prenom", prenom_);
    query.bindValue(":email", email_);
    query.bindValue(":role", role_);

    return (query.exec());
}

bool Employe::valideremail(const QString &mail)
{
    QRegularExpression mailRegex("[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}");

       return mailRegex.match(mail).hasMatch();
}
QString Employe::getDetailsByID(QString id)
{
    QString details;

    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYE WHERE id_E = :id_E");
    query.bindValue(":id_E", id);

    if (query.exec() && query.next()) {
        details += "id_E: " + query.value(0).toString() + "\n";
        details += "nom: " + query.value(1).toString() + "\n";
        details += "prenom: " + query.value(2).toString() + "\n";
        details += "email: " + query.value(3).toString() + "\n";
        details += "role: " + query.value(4).toString() + "\n";

    }

    return details;
}

void Employe::exportToPDF(QString id)
{
    QString employeeInfo = getDetailsByID(id);

        if (!employeeInfo.isEmpty()) {
            QPrinter printer;
            printer.setOutputFormat(QPrinter::PdfFormat);
            QString fileName = "employee_info.pdf";
            printer.setOutputFileName(fileName);

            QPainter painter;
            painter.begin(&printer);

            painter.drawText(100, 100, employeeInfo);

            painter.end();
            // Close the printer after use
            printer.setOutputFileName("");

            QMessageBox::information(nullptr, QObject::tr("Export Successful"),
                                     QObject::tr("Details exported to %1").arg(fileName),
                                     QMessageBox::Ok);
        } else {
            qDebug() << "Error: Employee not found or an error occurred.";
            QMessageBox::critical(nullptr, QObject::tr("Export Error"),
                                  QObject::tr("Employee not found or an error occurred."),
                                  QMessageBox::Ok);
        }
}



