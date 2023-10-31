#include "forniseur.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>



Forniseur::Forniseur()
{
    id = 0;
    nom = "";
    email = "";
    tel = 0;
    adresse = "";
}

Forniseur::Forniseur(int id,QString nom,QString email,int tel,QString adresse)
{
    this->id = id;
    this->nom = nom;
    this->email = email;
    this->tel = tel;
    this->adresse = adresse;
}

int Forniseur::getid(){ return id; }
QString Forniseur::getnom(){ return nom; }
QString Forniseur::getemail(){ return email; }
int Forniseur::gettel(){ return tel; }
QString Forniseur::getadresse(){ return adresse; }

void Forniseur::setid(int id){ this->id = id; }
void Forniseur::setnom(QString nom){ this->nom = nom; }
void Forniseur::setemail(QString email){ this->email = email;}
void Forniseur::settel(int tel){ this->tel = tel; }
void Forniseur::setadresse(QString adresse){ this->adresse = adresse; }

bool Forniseur::ajouter()
{

    QSqlQuery query;
    query.prepare("INSERT INTO FORNISUER (ID,NOM, EMAIL,TEL,ADRESSE) "
                  "VALUES (:id,:nom, :email, :tel, :adresse)");

    QString idString = QString::number(id);
    QString telString = QString::number(tel);


    query.bindValue(":id", idString);
    query.bindValue(":nom", nom);
    query.bindValue(":email", email);
    query.bindValue(":tel", telString);
    query.bindValue(":adresse", adresse);

    if (query.exec()) {
        qDebug() << "Insertion successful";
        return true;
    } else {
        QSqlError error = query.lastError();
        if (error.isValid()) {
            qDebug() << "Error: " << error.text();
        }
        return false;
    }
}

bool Forniseur::modifier()
{
    QString dt = QDateTime::currentDateTime().toString("yyyy-MM-dd");

    QSqlQuery query;
    query.prepare("UPDATE FORNISUER SET  NOM = :nom, EMAIL = :email ,TEL = :tel ,ADRESSE = :adresse WHERE ID = :id");

    QString idString = QString::number(id);
    QString telString = QString::number(tel);


    query.bindValue(":id", idString);
    query.bindValue(":nom", nom);
    query.bindValue(":email", email);
    query.bindValue(":tel", telString);
    query.bindValue(":adresse", adresse);

    if (query.exec()) {
        int rowsAffected = query.numRowsAffected();
        if (rowsAffected == 1) {
            qDebug() << "Record with ID " << this->id << " was successfully updated.";
            return true;
        } else {
            qDebug() << "No records were updated. ID " << this->id << " may not exist in the database.";
            return false;
        }
    } else {
        QSqlError error = query.lastError();
        if (error.isValid()) {
            qDebug() << "Error updating record: " << error.text();
        }
        return false;
    }
}



bool Forniseur::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM FORNISUER WHERE ID = :id");
    query.bindValue(":id", id);
    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            return true;
        }
    }
    return false;
}

QSqlQueryModel *Forniseur::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM FORNISUER");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TEL"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ADRESSE"));
    return model;
}
