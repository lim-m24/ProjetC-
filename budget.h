#ifndef BUDGET_H
#define BUDGET_H
#include<QString>
#include<QSqlQuery>
#include <QSqlQueryModel>

class Budget
{
public:
    Budget();
    Budget(int,float,float);

    int getid();
    float getprix_e();
    float getprix_r();
    QString getdate();

    void setid(int);
    void setprix_e(float);
    void setprix_r(float);

    bool ajouter();
    bool modifier();
    QSqlQueryModel* afficher(int);
    bool supprimer(int);
private:
    int id;
    float prix_e, prix_r;
};

#endif // BUDGET_H
