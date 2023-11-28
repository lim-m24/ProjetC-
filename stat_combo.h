#ifndef STAT_COMBO_H
#define STAT_COMBO_H
#include <QDialog>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>


QT_CHARTS_USE_NAMESPACE

namespace Ui {
class stat_combo;
}
class stat_combo : public QDialog
{
    Q_OBJECT

public:
    explicit stat_combo(QWidget *parent = nullptr);
    ~stat_combo();

    //QChartView *chartView ;
    void choix_bar();
    void choix_pie();
    void setPourcentage_cash(double pourcentage);
    void setPourcentage_carte(double pourcentage);



private:
    Ui::stat_combo *ui;
    QChartView *chartView;
    double Pourcentage_cash;
    double Pourcentage_carte;

};

#endif // STAT_COMBO_H
