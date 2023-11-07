#include "budget.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>



Budget::Budget()
{
    id = 0;
    prix_e = 0;
    prix_r = 0;
}

Budget::Budget(int id, float prix_e, float prix_r)
{
    this->id = id;
    this->prix_e = prix_e;
    this->prix_r = prix_r;
}

int Budget::getid() { return id; }
float Budget::getprix_e() { return prix_e; }
float Budget::getprix_r() { return prix_r; }

void Budget::setid(int id) { this->id = id; }
void Budget::setprix_e(float prix_e) { this->prix_e = prix_e; }
void Budget::setprix_r(float prix_r) { this->prix_r = prix_r; }

bool Budget::ajouter()
{
    QString dt = QDateTime::currentDateTime().toString("dd-MM-yyyy");

    QSqlQuery query;
    query.prepare("INSERT INTO BUDGET (ID,PRIX_E, PRIX_R,DATE_B) "
                  "VALUES (:id,:Prix_e, :Prix_r, :date_b)");

    QString idString = QString::number(id);
    QString prixEString = QString::number(prix_e);
    QString prixRString = QString::number(prix_r);

    query.bindValue(":id", idString);
    query.bindValue(":Prix_e", prixEString);
    query.bindValue(":Prix_r", prixRString);
    query.bindValue(":date_b", dt);

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

bool Budget::modifier()
{
    QString dt = QDateTime::currentDateTime().toString("dd.MM.yyyy");

    QSqlQuery query;
    QMessageBox msgBox;
    if(prix_e==0.0){
        if(prix_r==0.0){
            msgBox.setText("No New value added");
        }else{
            query.prepare("UPDATE BUDGET SET PRIX_R = :Prix_r, DATE_B = :date_b WHERE ID = :id");
        }
    }else {
        if(prix_r==0.0){
            query.prepare("UPDATE BUDGET SET PRIX_E = :Prix_e, DATE_B = :date_b WHERE ID = :id");
        }else{
            query.prepare("UPDATE BUDGET SET PRIX_E = :Prix_e, PRIX_R = :Prix_r, DATE_B = :date_b WHERE ID = :id");
        }
    }

    QString idString = QString::number(id);
    QString prixEString = QString::number(prix_e);
    QString prixRString = QString::number(prix_r);

    query.bindValue(":id", idString);
    query.bindValue(":Prix_e", prixEString);
    query.bindValue(":Prix_r", prixRString);
    query.bindValue(":date_b", dt);

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



bool Budget::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM BUDGET WHERE ID = :id");
    query.bindValue(":id", id);
    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            return true;
        }
    }
    return false;
}

QSqlQueryModel *Budget::afficher(int sort)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    switch (sort) {
        case 0 :
            model->setQuery("SELECT * FROM BUDGET ORDER BY ID DESC");
            break;
        case 1 :
            model->setQuery("SELECT * FROM BUDGET ORDER BY ID ASC");
            break;
        case 2 :
            model->setQuery("SELECT * FROM BUDGET ORDER BY PRIX_E DESC");
            break;
        case 3 :
            model->setQuery("SELECT * FROM BUDGET ORDER BY PRIX_E ASC");
            break;
        case 4 :
            model->setQuery("SELECT * FROM BUDGET ORDER BY PRIX_R DESC");
            break;
        case 5 :
            model->setQuery("SELECT * FROM BUDGET ORDER BY PRIX_R ASC");
            break;
        case 6 :
            model->setQuery("SELECT * FROM BUDGET ORDER BY DATE_B DESC");
            break;
        case 7 :
            model->setQuery("SELECT * FROM BUDGET ORDER BY DATE_B ASC");
            break;
    }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PRIX_E"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRIX_R"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE_B"));
    return model;
}
