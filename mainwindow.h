#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "forniseur.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajouter_clicked();

    void on_supprimer_clicked();

    void on_Modifier_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);
    void updateNomLabel(const QString &text);
    void tempSlot(const QString &text);


    void on_pushButton_2_clicked();


    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    Forniseur F;
};
#endif // MAINWINDOW_H
