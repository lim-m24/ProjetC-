#include "client.h"
#include <iostream>
#include<QRegularExpression>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include<QPainter>
#include<QPrinter>
#include <QPdfWriter>
Client::Client(QString nom_, QString tel_, int id_, int modePaiement_, double montantDepense_)
{
    nom = nom_;
    tel = tel_;
    id = id_;
    modePaiement = modePaiement_;
    montantDepense = montantDepense_;
}

QSqlQueryModel* Client::afficherClients(int sort)
{
    QSqlQueryModel* model = new QSqlQueryModel;

    QString query;


        switch (sort) {
            case 1:
                query = "SELECT * FROM table_des_clients ORDER BY MONTANTDEPENSE DESC;";
                break;
            case 2:
                query = "SELECT * FROM table_des_clients ORDER BY ID ASC;";
                break;
            case 3:
                query = "SELECT * FROM table_des_clients ORDER BY NOMCLIENT DESC;";
                break;
            default:
                query = "SELECT * FROM table_des_clients;";
        }
        model->setQuery(query);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("tel"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("modePaiement"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("montantDepense"));

    return model;
}

bool Client::ajouter()
{
    QSqlQuery query;
    QSqlQuery id_query;

    QString id_ = QString::number(id);

    id_query.prepare("SELECT id from table_des_clients where id = :id");
    id_query.bindValue(":id", id_);
    id_query.exec();

    if (id_query.next() and id_query.value(0).toInt()) {
        QMessageBox::critical(nullptr, QObject::tr("id déjà existant"),
                    QObject::tr("Entrez un id unique.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
        return -1;
    }
    else {
        query.prepare("INSERT INTO table_des_clients"
                      " (DATE_ACHAT,ID, NOMCLIENT, NUMCLIENT, MODEPAIEMENT, MONTANTDEPENSE)"
                      " VALUES (CURRENT_DATE,:id, :nom, :tel, :mode, :montant);");

        QString modePaiement_ = QString::number(modePaiement);
        QString montant_ = QString::number(montantDepense);

        query.bindValue(":id", id_);
        query.bindValue(":nom", nom);
        query.bindValue(":tel", tel);
        query.bindValue(":mode", modePaiement_);
        query.bindValue(":montant", montant_);
        query.exec();
        QMessageBox::information(nullptr, QObject::tr("ajout avec succes"),
                    QObject::tr("ajout avec succes.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
        return 1;
    }
}

bool Client::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE table_des_clients SET NOMCLIENT=:nom,NUMCLIENT=:tel,MODEPAIEMENT=:mode,MONTANTDEPENSE=:montant WHERE ID=:id");


    QString id_ = QString::number(id);
    QString modePaiement_ = QString::number(modePaiement);
    QString montant_ = QString::number(montantDepense);

    query.bindValue(":id", id_);
    query.bindValue(":nom", nom);
    query.bindValue(":tel", tel);
    query.bindValue(":mode", modePaiement_);
    query.bindValue(":montant", montant_);

    return (query.exec());
}



// Fonction pour afficher l'historique des achats pour un client
QSqlQueryModel* Client::afficherHistoriqueAchats()
{
    QSqlQueryModel* model = new QSqlQueryModel;
    model->setQuery("SELECT DATE_ACHAT, MONTANTDEPENSE, ID FROM table_des_clients;");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Date d'achat"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("montant"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("id"));

    return model;
}
QString Client::getDetailsByID(QString id)
{
    QString details;

    QSqlQuery query;
    query.prepare("SELECT * FROM table_des_clients WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        details += "id: " + query.value(0).toString() + "\n";
        details += "nom: " + query.value(1).toString() + "\n";
        details += "tel: " + query.value(2).toString() + "\n";
        details += "mode: " + query.value(3).toString() + "\n";
        details += "montant: " + query.value(4).toString() + "\n";

    }

    return details;
}
void Client::exportToPDF(QString id)
{
    QString employeeInfo = getDetailsByID(id);

        if (!employeeInfo.isEmpty()) {
            QPrinter printer;
            printer.setOutputFormat(QPrinter::PdfFormat);
            QPdfWriter pdf("C:/sers/HP/OneDrive - ESPRIT/Doc2.pdf");

            //QString fileName = "employee_info.pdf";
            //printer.setOutputFileName(fileName);


            QPainter painter;
            painter.begin(&printer);

            painter.drawText(100, 100, employeeInfo);

            painter.end();
            // Close the printer after use
            printer.setOutputFileName("");

            //QMessageBox::information(nullptr, QObject::tr("Export Successful"),
                                    // QObject::tr("Details exported to %1").arg(fileName),
                                    // QMessageBox::Ok);
        } else {
            qDebug() << "Error: Employee not found or an error occurred.";
            QMessageBox::critical(nullptr, QObject::tr("Export Error"),
                                  QObject::tr("Employee not found or an error occurred."),
                                  QMessageBox::Ok);
        }
}
