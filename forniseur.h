#ifndef Forniseur_H
#define Forniseur_H
#include<QString>
#include<QSqlQuery>
#include <QSqlQueryModel>

class Forniseur
{
public:
    Forniseur();
    Forniseur(int,QString,QString,int,QString);

    int getid();
    QString getnom();
    QString getemail();
    int gettel();
    QString getadresse();

    void setid(int);
    void setnom(QString);
    void setemail(QString);
    void settel(int);
    void setadresse(QString);

    bool ajouter();
    bool modifier();
    QSqlQueryModel* afficher(int);
    bool supprimer(int);
private:
    int id,tel;
    QString nom, email,adresse;
};

#endif // Forniseur_H
