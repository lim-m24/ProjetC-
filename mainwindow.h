#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "produit.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

namespace UI {class MainWindow;}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajout_clicked();
    void on_modifier_clicked();
    void on_supprimer_clicked();
    void on_afficher_clicked();
    void on_chercher_clicked();
    void on_pdf_clicked();
    void on_rupture_stock_clicked();
    void on_statistique_clicked();
    void on_comboBox_activated(const QString &arg1);

private:
    Ui::MainWindow *ui;
    produit p;
};

#endif // MAINWINDOW_H
