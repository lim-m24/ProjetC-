#include "budget.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"



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
    QSqlQuery query2;

    query.prepare("INSERT INTO BUDGET (ID, PRIX_E, PRIX_R, DATE_B) "
                  "VALUES (:id, :Prix_e, :Prix_r, :date_b)");

    QString idString = QString::number(id);
    QString prixEString = QString::number(prix_e);
    QString prixRString = QString::number(prix_r);

    query.bindValue(":id", idString);
    query.bindValue(":Prix_e", prixEString);
    query.bindValue(":Prix_r", prixRString);
    query.bindValue(":date_b", dt);

    if (prix_r==0) {
        query2.prepare("UPDATE BUDGET SET STATUS = 'En cours' WHERE ID = :id");
    } else {
        query2.prepare("UPDATE BUDGET SET STATUS = 'Termine' WHERE ID = :id");
    }

    query2.bindValue(":id", idString);

    bool insertionSuccess = query.exec();
    bool updateSuccess = query2.exec();

    if (insertionSuccess && updateSuccess) {
        qDebug() << "Insertion and update successful";
        return true;
    } else {
        QSqlError error = query.lastError();
        if (error.isValid()) {
            qDebug() << "Error: " << error.text();
        }
        return false;
    }

}

bool Budget::modifier(bool wiw,bool wiw2) {
    QString dt = QDateTime::currentDateTime().toString("dd-MM-yyyy");
    QSqlQuery query;
    QSqlQuery query2;
    QMessageBox msgBox;

    QStringList updateFields;
    QString updateQuery;

    if (wiw2) {
        updateFields.append("PRIX_E = :Prix_e");
    }

    if (wiw) {
        updateFields.append("PRIX_R = :Prix_r");
    }


    if (!updateFields.isEmpty()) {
        updateQuery = "UPDATE BUDGET SET " + updateFields.join(", ") + ", DATE_B = :date_b WHERE ID = :id";
        query.prepare(updateQuery);

        QString idString = QString::number(id);
        QString prixEString = QString::number(prix_e);
        QString prixRString = QString::number(prix_r);

        query.bindValue(":id", idString);

        if (wiw2) {
            query.bindValue(":Prix_e", prixEString);
        }

        if (wiw) {
            query.bindValue(":Prix_r", prixRString);
        }

        query.bindValue(":date_b", dt);

        if (prix_r==0) {
            query2.prepare("UPDATE BUDGET SET STATUS = 'En cours' WHERE ID = :id");
        } else {
            query2.prepare("UPDATE BUDGET SET STATUS = 'Termine' WHERE ID = :id");
        }

        if (query.exec()) {
            int rowsAffected = query.numRowsAffected();
            if (rowsAffected == 1) {
                qDebug() << "Record with ID " << this->id << " was successfully updated.";
                return true;
            } else {
                qDebug() << "No records were updated. ID " << this->id << " may not exist in the database.";
                return false;
            }
        } else
        {
            QSqlError error = query.lastError();
            if (error.isValid()) {
                qDebug() << "Error updating record: " << error.text();
            }
            return false;
        }
    } else
    {
        msgBox.setText("No new value added");
        msgBox.exec();
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
bool Budget::annuler(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE BUDGET SET STATUS = 'Annule' WHERE ID = :id");
    query.bindValue(":id", id);
    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            return true;
        }
    }
    return false;
}


QSqlQueryModel *Budget::afficher(int sort) {
    QSqlQueryModel *model = new QSqlQueryModel();

    QString orderBy;
    switch (sort) {
        case 0:
            orderBy = "ID ASC";
            break;
        case 1:
            orderBy = "ID DESC";
            break;
        case 2:
            orderBy = "PRIX_E DESC";
            break;
        case 3:
            orderBy = "PRIX_E ASC";
            break;
        case 4:
            orderBy = "PRIX_R DESC";
            break;
        case 5:
            orderBy = "PRIX_R ASC";
            break;
        case 6:
            orderBy = "DATE_B DESC";
            break;
        case 7:
            orderBy = "DATE_B ASC";
            break;
    }

    QString queryStr = "SELECT * FROM BUDGET ORDER BY " + orderBy;
    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PRIX_E"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRIX_R"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE_B"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));

    return model;
}
QSqlQueryModel *Budget::afficher_stock(){
    QSqlQueryModel *model = new QSqlQueryModel();
    QString queryStr = "SELECT "
                       "    PRODUIT.ID_P, "
                       "    PRODUIT.NOM, "
                       "    PRODUIT.PRIX_V, "
                       "    PRODUIT.PRIX_A, "
                       "    PRODUIT.QTE "
                       "FROM "
                       "    PRODUIT "
                       "JOIN "
                       "    (SELECT PRODUIT.ID_P AS PID, SUM(FACTURE.QTE) AS TotalQte "
                       "     FROM PRODUIT "
                       "     JOIN FACTURE ON FACTURE.PRODUIT = PRODUIT.ID_P "
                       "     GROUP BY PRODUIT.ID_P) SubQuery "
                       "ON "
                       "    PRODUIT.ID_P = SubQuery.PID "
                       "WHERE "
                       "    PRODUIT.QTE < SubQuery.TotalQte;";

    model->setQuery(queryStr);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_P"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRIX_V"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRIX_A"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("QTE"));
    return model;
}
QSqlQueryModel *Budget::afficher_demande(){
    QSqlQueryModel *model = new QSqlQueryModel();
    QString queryStr = "SELECT FACTURE.ID_F, PRODUIT.NOM ,FACTURE.QTE FROM FACTURE JOIN PRODUIT ON FACTURE.PRODUIT = PRODUIT.ID_P WHERE FACTURE.ETAT = 'commande';";
    model->setQuery(queryStr);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_F"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PRODUIT"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("QTE"));
    return model;
}

QStandardItemModel* Budget::afficher_forniseur(const int& selectedProductId, int quantityNeeded) {
    QStandardItemModel *model = new QStandardItemModel();

    QSqlQuery query;
    query.prepare("SELECT ID_F, Qte, Prix FROM DISTRIBUTION WHERE ID_P = :id AND Qte > 0 ORDER BY Prix ASC");
    query.bindValue(":id", selectedProductId);

    if (query.exec()) {
        int totalQuantityBought = 0;
        double totalCost = 0.0;

        model->setHorizontalHeaderLabels({"ID_F", "ID_P", "Qte Bought", "Total Cost"});

        while (query.next() && totalQuantityBought < quantityNeeded) {
            int availableQuantity = query.value("Qte").toInt();
            int quantityToBuy = qMin(quantityNeeded - totalQuantityBought, availableQuantity);

            totalQuantityBought += quantityToBuy;
            totalCost += quantityToBuy * query.value("Prix").toDouble();

            QList<QStandardItem*> rowItems;
            rowItems << new QStandardItem(QString::number(query.value("ID_F").toInt()));
            rowItems << new QStandardItem(QString::number(selectedProductId));
            rowItems << new QStandardItem(QString::number(quantityToBuy));
            rowItems << new QStandardItem(QString::number(totalCost));

            model->appendRow(rowItems);
        }

    }
    return model;
}


