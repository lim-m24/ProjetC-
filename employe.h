#ifndef EMPLOYE_H
#define EMPLOYE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QObject>
#include <QSqlError>
#include <QTableView>
class Employe
{
    int id_E;
    QString nom, prenom,email,role,username,password;
public:
    Employe(int id_E=0,QString nom="",QString prenom="",QString email="",QString role="",QString username="",QString password="");
    int getid_E(){return id_E;}
    QString getnom(){return nom;}
    QString getprenom(){return prenom;}
    QString getemail(){return email;}
    QString getrole(){return role;}
    QString getusername(){return username;}
    QString getpassword(){return password;}
    void setid_E(int id_E_){id_E=id_E_;}
    void setnom(QString nom_){nom=nom_;}
    void setprenom(QString prenom_){prenom=prenom_;}
    void setemail(QString email_){email=email_;}
    void setrole(QString role_){this->role=role_;}
    void setusername(QString username){this->username=username;}
    void setpassword(QString password){this->password=password;}
    bool ajouter();
    bool Modifier();
    QSqlQueryModel* afficher(int);
    bool valideremail(const QString &mail);
    bool supprimer(int);
    void exportToPDF(QString id);
    QString getDetailsByID(QString id);

};
#endif // EMPLOYE_H
