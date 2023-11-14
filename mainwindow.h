#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "budget.h"

#include <QMainWindow>
////////


#include<QtCharts>
#include<QChartView>
#include<QPieSeries>
#include<QPieSlice>
using namespace QtCharts;


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_lineEditSearch_textChanged(const QString &arg1);

    void on_calendarWidget_selectionChanged();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();
    void updateTotalCostLabel();


    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_comboBox_2_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    Budget B;
};
#endif // MAINWINDOW_H
