#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet2A");
db.setUserName("mouhanned");//inserer nom de l'utilisateur
db.setPassword("esprit18");//inserer mot de passe de cet utilisateur

return  (db.open());
}
