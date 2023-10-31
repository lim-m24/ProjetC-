#include "connection.h"

Connection::Connection()
{

}

bool Connection::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("PROJETCPP");
    db.setUserName("aziz");//inserer nom de l'utilisateur
    db.setPassword("aziz");//inserer mot de passe de cet utilisateur

    return (db.open());
}
