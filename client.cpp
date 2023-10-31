#include "client.h"
#include <iostream>

Client::Client(QString nom_, QString tel_, int id_, int modePaiement_, double montantDepense_)
{
    nom = nom_;
    tel = tel_;
    id = id_;
    modePaiement = modePaiement_;
    montantDepense = montantDepense_;
}

QSqlQueryModel* afficherClients()
{
    QSqlQueryModel* model = new QSqlQueryModel;
    model->setQuery("select * from table_des_clients");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("tel"));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("modePaiement"));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("montantDepense"));

    return model;
}

bool Client::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO table_des_clients"
                  " (ID, NOMCLIENT, NUMCLIENT, MODEPAIEMENT, MONTANTDEPENSE)"
                  " VALUES (:id, :nom, :tel, :mode, :montant);");

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


