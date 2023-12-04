#include "connexion.h"
#include <QSqlDatabase>
Connexion::Connexion(){}

bool Connexion::createconnect()
{
    db=QSqlDatabase::addDatabase("QODBC");
    bool test=false;
    db.setDatabaseName("Source_Produit2A");
    db.setUserName("kenza");//inserer nom de l'utilisateur
    db.setPassword("esprit18");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;
    return  test;
}
void Connexion::closeConnection(){db.close();}


