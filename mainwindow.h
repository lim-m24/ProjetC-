#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "employe.h"

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


    void on_pushButton_3_clicked();

    void on_upmail_clicked();

    void on_pushButton_4_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_5_clicked();

    void on_eye_clicked();

private:
    Ui::MainWindow *ui;
    Employe E;
};

#endif // MAINWINDOW_H
