#include <QString>
#include <QSqlQuery>
#include <QtDebug>
#include <QObject>
#include <QSqlError>
#include <QMessageBox>
#include "mainwindow.h"
#include "produit.h"
#include "connexion.h"




produit::~produit()
{

}
produit::produit()
{
    id=0;
    prix_vente=0.0;
    prix_achat=0.0;
    quantite=0;
}


produit::produit(int id,QString nom,QString description,float prix_vente ,float prix_achat,int quantite)
{

        this->id=id;
        this->nom=nom;
        this->description=description;
        this->prix_vente=prix_vente;
        this->prix_achat=prix_achat;
        this->quantite=quantite;
}


bool produit::ajouter()
{
        QSqlQuery q;
        QString res = QString::number(id);
        QString respv = QString::number(prix_vente);
        QString respa = QString::number(prix_achat);
        QString resqu = QString::number(quantite);
        q.prepare("INSERT INTO PRODUIT (ID,NOM,DESCRIPTION,PRIX_VENTE,PRIX_ACHAT,QUANTITE) VALUES (:i,:m,:d,:pv,:pa,:qu);");
        q.bindValue(":i",res);
        q.bindValue(":m",nom);
        q.bindValue(":d",description);
        q.bindValue(":pv",respv);
        q.bindValue(":pa",respa);
        q.bindValue(":qu",resqu);
        if (q.exec()) {
            qDebug() << "Insertion successful";
            return true;
        } else {
            QSqlError error = q.lastError();
            if (error.isValid()) {
                qDebug() << "Error: " << error.text();
            }
            return false;
        };
}

QSqlQueryModel * produit::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PRODUIT");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DESCRIPTION"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRIX_VENTE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PRIX_ACHAT"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("QUANTITE"));

    return model;
}

bool produit::supprimer(int id)
{
    QSqlQuery q;
    QString res=QString::number(id);
    q.prepare("DELETE FROM PRODUIT WHERE ID = :i;");
    q.bindValue(":i",res);

    return q.exec();
}

bool produit::modifier()
{

         QSqlQuery update_query ;
         QString res = QString::number(id);
         QString respv = QString::number(prix_vente);
         QString respa = QString::number(prix_achat);
         QString resqu = QString::number(quantite);
          update_query.prepare("UPDATE PRODUIT SET NOM= :mm,DESCRIPTION= :md,PRIX_VENTE= :mpv,PRIX_ACHAT= :mpa ,QUANTITE= :mqu WHERE ID= :mi");

                    update_query.bindValue(":mi", res);
                    update_query.bindValue(":mm", nom);
                    update_query.bindValue(":md", description);
                    update_query.bindValue(":mpv",respv);
                    update_query.bindValue(":mpa", respa);
                    update_query.bindValue(":mqu", resqu);
                    return  update_query.exec();
     }



QSqlQueryModel * produit::trier(QString tri)
{
    QSqlQueryModel * model = new QSqlQueryModel();

    if(tri == "Trier")
    {
        model->setQuery("SELECT * FROM PRODUIT ");
    }
    else if (tri == "Trie par ID")
    {
        model->setQuery("SELECT * FROM PRODUIT ORDER BY ID");
    }
    else if (tri == "Trie par quantite")
    {
        model->setQuery("SELECT * FROM PRODUIT ORDER BY QUANTITE");
    }
    else if (tri == "Trie par prix_d'achat")
        {
            model->setQuery("SELECT * FROM PRODUIT ORDER BY PRIX_ACHAT");
        }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DESCRIPTION"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRIX_VENTE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PRIX_ACHAT"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("QUANTITE"));

    return model;

}


QSqlQueryModel *produit::recherche(QString i)
 {
    QSqlQueryModel * model= new QSqlQueryModel();
    model->setQuery("select * from PRODUIT where ID LIKE '"+i+"%' or NOM LIKE '"+i+"%' or DESCRIPTION LIKE '"+i+"%' or PRIX_VENTE LIKE '"+i+"%' or PRIX_ACHAT LIKE '"+i+"%' or QUANTITE LIKE '"+i+"%'");


    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DESCRIPTION"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRIX_VENTE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PRIX_ACHAT"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("QUANTITE"));


return model;
}

bool produit::estEnRuptureDeStock() {

    return quantite ;
}


QSqlDatabase produit::db = QSqlDatabase::addDatabase("QODBC");
QVector<QPointF> produit::getLineChartData()
{
    QVector<QPointF> dataPoints;
    db.setDatabaseName("Source_Produit2A");
    if (!db.open()) {
            qDebug() << "Failed to open the database";
            return dataPoints;
        }

        QSqlQuery query("SELECT QUANTITE, PRIX_ACHAT FROM PRODUIT");
        if (!query.exec()) {
            qDebug() << "Query failed:" << query.lastError().text();
            db.close();
            return dataPoints;
        }

        while (query.next()) {
            int quantite = query.value(0).toInt();
            float prixVente = query.value(1).toFloat();
            dataPoints.append(QPointF(quantite, prixVente));
        }
        db.close();

        return dataPoints;
}
