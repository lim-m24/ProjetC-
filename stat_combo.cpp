#include "stat_combo.h"
#include "ui_stat_combo.h"
#include <QTextStream>
#include <QDebug>
#include <QApplication>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QSqlQuery>

stat_combo::stat_combo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::stat_combo)
{
    ui->setupUi(this);
}

stat_combo::~stat_combo()
{
    delete ui;
}

void stat_combo:: setPourcentage_cash(double pourcentage)
{
    Pourcentage_cash = pourcentage;
    QString text = QString("cash: %1%").arg(pourcentage);
    ui->label_cash->setText(text);
}

void stat_combo::setPourcentage_carte(double pourcentage)
{
    Pourcentage_carte = pourcentage;
    QString text = QString("carte: %1%").arg(pourcentage);
    ui->label_carte->setText(text);
}


void stat_combo::choix_bar()
{
    QSqlQuery q1, q2, q3, q4, q5;
    qreal tot = 0, c1 = 0, c2 ;

    q1.prepare("SELECT * FROM table_des_clients ");
    q1.exec();

    q2.prepare("SELECT * FROM  table_des_clients WHERE ETAT='cash'");
    q2.exec();

    q3.prepare("SELECT * FROM FROM table_des_clients WHERE ETAT='carte'");
    q3.exec();





    // Ajoutez d'autres requêtes pour les autres conditions si nécessaire

    while (q1.next()) { tot++; }
    while (q2.next()) { c1++; }
    while (q3.next()) { c2++; }

    // Calculez les pourcentages
    c1 = (c1 / tot) * 100;
    c2 = (c2 / tot) * 100;



    setPourcentage_cash(c1);
    setPourcentage_carte(c2);


    QString percentageC1 = QString::number(c1, 'f', 2) + "%";
    QString percentageC2 = QString::number(c2, 'f', 2) + "%";



    QBarSet *set0 = new QBarSet("cash");
    QBarSet *set1 = new QBarSet("carte");
 QBarSet *set2 = new QBarSet("autre");

    *set0 << c1;
    *set1 << c2;
    *set2 << c2;

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);


    QChart *chart = new QChart();
    chart->addSeries(series);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    chart->setAxisX(axis, series);

    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, QRgb(0xffffff));
    pal.setColor(QPalette::WindowText, QRgb(0x404044));
    qApp->setPalette(pal);

    chartView = new QChartView(chart, ui->label_cash);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(500, 400);
    chartView->show();
}

void stat_combo::choix_pie()
{
    QSqlQuery q1, q2, q3;
    qreal tot = 0, c1 = 0, c2 = 0;

    q1.prepare("SELECT * FROM table_des_clients");
    q1.exec();

    q2.prepare("SELECT * FROM table_des_clients WHERE ETAT='cash'");
    q2.exec();

    q3.prepare("SELECT * FROM table_des_clients WHERE ETAT='carte'");
    q3.exec();



    // Ajoutez d'autres requêtes pour les autres conditions si nécessaire

    while (q1.next()) { tot++; }
    while (q2.next()) { c1++; }
    while (q3.next()) { c2++; }

    // Calculez les pourcentages
    c1 = (c1 / tot) * 100;
    c2 = (c2 / tot) * 100;



    setPourcentage_cash(c1);
    setPourcentage_carte(c2);



    QString percentageC1 = QString::number(c1, 'f', 2) + "%";
    QString percentageC2 = QString::number(c2, 'f', 2) + "%";



    QPieSeries *series = new QPieSeries();
    series->append("cash", c1);
    series->append("carte", c2);




    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->show();

    chartView = new QChartView(chart, ui->label_cash);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(500, 400);
    chartView->show();
}
