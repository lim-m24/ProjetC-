#ifndef PRODUIT_H
#define PRODUIT_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class produit
{
    public:
       produit();
       produit(int ,QString ,QString ,float  ,float ,int );
       ~produit();

       int Getid() { return id; }
       void Setid(int val) { id = val; }
       QString Getnom() { return nom; }
       void Setnom(QString val) { nom = val; }
       QString Getdescription() { return description; }
       void Setdescription(QString val) { description = val; }
       float Getprix_vente() { return prix_vente; }
       void Setprix_vente(float val) { prix_vente = val; }
       float Getprix_achat() { return prix_achat; }
       void Setprix_achat(float val) { prix_achat = val; }
       int Getquantite() { return quantite; }
       void Setquantite(int val) { quantite = val; }

       bool ajouter();
       QSqlQueryModel * afficher();
       bool supprimer(int);
       bool modifier();
       QSqlQueryModel * trier(QString);
       QSqlQueryModel *recherche(QString i);
       bool estEnRuptureDeStock() ;

       protected:

       private:
       int id;
       QString nom;
       QString description;
       float prix_vente;
       float prix_achat;
       int quantite;




};

#endif // PRODUIT_H
