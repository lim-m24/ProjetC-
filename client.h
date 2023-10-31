#ifndef ETUDIANT_H
#define ETUDIANT_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>

enum {
    PAIEMENT_CARTE,
    PAIEMENT_ESPECE,
};

class Client
{
QString nom;
QString tel;
int id;
int modePaiement;
double montantDepense;

public:
    Client(QString nom_="", QString tel_="", int id_=0, int modePaiement_=PAIEMENT_ESPECE, double montantDepense_=0.f);

    // GETTERS
    int getid(){return id;}
    QString getTel(){return tel;}
    QString getnom(){return nom;}
    int get_modePaiement(){return modePaiement;}
    double get_montantDepense(){return montantDepense;}
    // SETTERS
    void setid(int id ){this->id=id;}
    void setid(QString tel_){tel = tel_;}
    void setnom(QString n){nom=n;}
    void set_modePaiement(int modePaiement_) {modePaiement = modePaiement_;}
    void set_montantDepense(double montantDepense_){montantDepense=montantDepense_;}

    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
};

QSqlQueryModel* afficherClients();

#endif // ETUDIANT_H
