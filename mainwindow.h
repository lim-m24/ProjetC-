#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "client.h"
#include "arduino.h"
#include <QMainWindow>
#include<QtCharts>
#include<QChartView>
#include<QPieSeries>
#include<QPieSlice>
#include <stat_combo.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_label_objectNameChanged(const QString &objectName);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_lineEdit_6_textChanged(const QString &arg1);

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void update_label();   // slot permettant la mise à jour du label état de la lampe 1,
    // ce slot est lancé à chaque réception d'un message de Arduino


private:
    Ui::MainWindow *ui;
    Client c;
    stat_combo *s;
    QByteArray data; // variable contenant les données reçues

     Arduino A; // objet temporaire
};

#endif // MAINWINDOW_H
