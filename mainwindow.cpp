#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "budget.h"
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDoubleValidator>
#include <QSqlQuery>
#include <QString>
#include <QSqlError>
#include <QBarSet>
#include <QBarSeries>
#include <QChart>
#include <QChartView>
#include <QBarCategoryAxis>

#include <QStringList>

#include <QSqlQuery>
#include <QSqlRecord>

#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QFileDialog>



int sort;
using namespace QtCharts;
QStandardItemModel *model2;
QStandardItemModel *model3;
QStandardItemModel *model4;
int selectedProductId;
int quantityNeeded;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showMaximized();
    sort = 0;

    ui->lineEdit_id->setValidator(new QIntValidator(1,99999999,this));
    ui->lineEdit_id_2->setValidator(new QIntValidator(1,99999999,this));
    ui->lineEdit_id_3->setValidator(new QIntValidator(1,99999999,this));
    ui->lineEdit_pe->setValidator(new QDoubleValidator(0.0,99999.99,2,this));
    ui->lineEdit_pe_2->setValidator(new QDoubleValidator(0.0,99999.99,2,this));
    ui->lineEdit_pr->setValidator(new QDoubleValidator(0.0,99999.99,2,this));
    ui->lineEdit_pr_2->setValidator(new QDoubleValidator(0.0,99999.99,2,this));
    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, &MainWindow::on_lineEditSearch_textChanged);

    ui->tableView->setModel(B.afficher(sort));
    ui->tableView_stock->setModel(B.afficher_stock());
    ui->tableView_demande->setModel(B.afficher_demande());



    model2 = new QStandardItemModel(this);

    ui->tableView_estimate->setModel(model2);

    model2->setHorizontalHeaderLabels({"Product", "Qte"});
    //--------------------------------------------
    QStringList items;

     QSqlQuery query;
     query.exec("SELECT PRODUIT.NOM "
                "FROM PRODUIT "
                "JOIN (SELECT PRODUIT.ID_P AS PID, SUM(FACTURE.QTE) AS TotalQte "
                "      FROM PRODUIT "
                "      JOIN FACTURE ON FACTURE.PRODUIT = PRODUIT.ID_P "
                "      GROUP BY PRODUIT.ID_P) SubQuery "
                "ON PRODUIT.ID_P = SubQuery.PID "
                "WHERE PRODUIT.QTE < SubQuery.TotalQte;");

     while (query.next()) {
         QString item = query.value(0).toString();
         items.append(item);
     }

     ui->comboBox->clear();
     ui->comboBox->addItems(items);
     ///////////////////Bilan////////////////////

     model3 = B.afficher_forniseur(selectedProductId, quantityNeeded);
     ui->tableView_buy->setModel(model3);
     model4 = new QStandardItemModel(this);
     ui->tableView_buy->setModel(model4);
     model4->setHorizontalHeaderLabels({"ID_F", "ID_P", "Qte Bought", "Prix"});

     ///////////////////STAT////////////////////

     QSqlQuery query5("SELECT ID, PRIX_E, PRIX_R, DATE_B, STATUS FROM BUDGET");

     QBarSet *priceDifferencesSet = new QBarSet("Price Differences");

     QBarCategoryAxis *axis = new QBarCategoryAxis();  // Declaration of 'axis'

     while (query5.next()) {
         QDateTime date = query5.value("DATE_B").toDateTime();
         float estimatedPrice = query5.value("PRIX_E").toFloat();
         float actualPrice = query5.value("PRIX_R").toFloat();

         float difference = estimatedPrice - actualPrice;
         priceDifferencesSet->append(difference);

         // Assuming you want to label the X-axis with dates
         QString dateString = date.toString("yyyy-MM-dd");
         axis->append(dateString);
     }

     QBarSeries *series = new QBarSeries();
     series->append(priceDifferencesSet);

     QChart *chart = new QChart();
     chart->addSeries(series);
     chart->setTitle("Price Differences Between Estimated and Actual Prices");
     chart->setAnimationOptions(QChart::SeriesAnimations);

     axis->setTitleText("Date");  // Setting the title for the X-axis
     chart->createDefaultAxes();
     chart->setAxisX(axis, series);

     QChartView *chartView = new QChartView(chart);
     chartView->setParent(ui->horizontalFrame);





}

MainWindow::~MainWindow()
{
    delete ui;
}


//add
void MainWindow::on_pushButton_7_clicked()
{
    QSqlQuery query;
    int id = ui->lineEdit_id->text().toInt();
    float prix_e = ui->lineEdit_pe->text().toFloat();
    float prix_r = ui->lineEdit_pr->text().toFloat();

    Budget B(id, prix_e, prix_r);
    bool test = B.ajouter();


    QString idString = QString::number(id);


    QMessageBox msgBox;

    if (test) {
        msgBox.setText("Ajout avec succès");
        ui->tableView->setModel(B.afficher(sort));
    } else {
        msgBox.setText("Failed");
    }

    msgBox.exec();
}

//delete
void MainWindow::on_pushButton_8_clicked()
{
    Budget B1; B1.setid(ui->lineEdit_id_2->text().toInt());
    bool test=B1.supprimer(B1.getid());
    QMessageBox msgBox;
    if(test){
        msgBox.setText("Suppression avec succes");
        ui->tableView->setModel(B.afficher(sort));
    }
    else
        msgBox.setText("failed");
        msgBox.exec();
}
//update
void MainWindow::on_pushButton_9_clicked()
{
    bool wiw,wiw2;
    int id = ui->lineEdit_id_3->text().toInt();
    float prix_e = ui->lineEdit_pe_2->text().toFloat();
    float prix_r = ui->lineEdit_pr_2->text().toFloat();
    Budget B(id, prix_e, prix_r);
    {
        wiw2=true;
    }
    {
        wiw=true;
    }
    bool test = B.modifier(wiw,wiw2);
    QMessageBox msgBox;

    if (test) {
        msgBox.setText("Update avec succès");
        ui->tableView->setModel(B.afficher(sort));
    } else {
        msgBox.setText("Failed");
    }

    msgBox.exec();
}

void MainWindow::on_pushButton_10_clicked()
{
    if(sort==1){
        sort=0;
    }else{
        sort=1;
    }
    ui->tableView->setModel(B.afficher(sort));
}

void MainWindow::on_pushButton_11_clicked()
{
    if(sort==2){
        sort=3;
    }else{
        sort=2;
    }
    ui->tableView->setModel(B.afficher(sort));
}

void MainWindow::on_pushButton_12_clicked()
{
    if(sort==4){
        sort=5;
    }else{
        sort=4;
    }
    ui->tableView->setModel(B.afficher(sort));
}

void MainWindow::on_pushButton_13_clicked()
{
    if(sort==6){
        sort=7;
    }else{
        sort=6;
    }
    ui->tableView->setModel(B.afficher(sort));
}

void MainWindow::on_lineEditSearch_textChanged(const QString &searchText) {
    if (searchText.isEmpty()) {
        ui->tableView->setModel(B.afficher(sort));
    } else {
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM BUDGET WHERE ID LIKE '" + searchText + "%'");
        ui->tableView->setModel(model);
    }
}

void MainWindow::on_calendarWidget_selectionChanged()
{
    QDate selectedDate = ui->calendarWidget->selectedDate();

    QSqlQueryModel *model = new QSqlQueryModel();
    QString formattedDate = selectedDate.toString("dd-MM-yyyy");
    model->setQuery("SELECT * FROM BUDGET WHERE DATE_B = '" + formattedDate + "'");
    ui->tableView->setModel(model);
}

void MainWindow::on_pushButton_14_clicked()
{
    QString selectedValue = ui->comboBox->currentText();

    if (!selectedValue.isEmpty()) {
        QSqlQuery query;
        if (query.exec("SELECT QTE FROM PRODUIT WHERE NOM = '" + selectedValue + "';")) {
            if (query.next()) {
                QString qte_prod = query.value(0).toString();

                if (query.exec("SELECT SUM(FACTURE.QTE) FROM FACTURE JOIN PRODUIT ON FACTURE.PRODUIT = PRODUIT.ID_P WHERE PRODUIT.NOM = '" + selectedValue + "';")) {
                    if (query.next()) {
                        QString qte_commande = query.value(0).toString();

                        int qte_needed = qte_commande.toInt() - qte_prod.toInt();

                        if (query.exec("SELECT ID_P FROM PRODUIT WHERE NOM = '" + selectedValue + "';")) {
                            if (query.next()) {
                                int idprod = query.value(0).toInt();
                                QStandardItemModel* model3 = B.afficher_forniseur(idprod, qte_needed);
                                for (int row = 0; row < model3->rowCount(); ++row) {
                                    QList<QStandardItem*> rowItems;
                                    for (int col = 0; col < model3->columnCount(); ++col) {
                                        QStandardItem* item = model3->item(row, col)->clone();
                                        rowItems.append(item);
                                    }
                                    model4->appendRow(rowItems);
                                }
                                QList<QStandardItem*> rowItemsEstimate;
                                rowItemsEstimate << new QStandardItem(selectedValue);
                                rowItemsEstimate << new QStandardItem(QString::number(qte_needed));
                                model2->appendRow(rowItemsEstimate);
                                ui->tableView_buy->setModel(model4);
                                updateTotalCostLabel();
                            }
                        }
                    }
                }
            }
        }
        int now = ui->comboBox->findText(selectedValue);
        ui->comboBox->removeItem(now);
    }
}

void MainWindow::updateTotalCostLabel() {
    double totalCost = 0.0;

    for (int row = 0; row < model4->rowCount(); ++row) {
        QModelIndex index = model4->index(row, 3);
        totalCost += index.data().toDouble();
    }

    ui->totalCostLabel->setText(QString::number(totalCost)+" DT");
}






void MainWindow::on_pushButton_15_clicked()
{
    Budget B1; B1.setid(ui->lineEdit_id_2->text().toInt());
    bool test=B1.annuler(B1.getid());
    QMessageBox msgBox;
    if(test){
        msgBox.setText("Suppression avec succes");
        ui->tableView->setModel(B.afficher(sort));
    }
    else
        msgBox.setText("failed");
        msgBox.exec();
}


void MainWindow::on_pushButton_17_clicked()
{
    QSqlQuery query;
    query.exec("SELECT MAX(ID) FROM BUDGET");
    query.next();
    int maxID = query.value(0).toInt();
    int id=maxID+1;

    QString cleanedText = ui->totalCostLabel->text().remove(QRegExp("[^0-9.]+"));
    bool conversionOk;
    float prix_e = cleanedText.toFloat(&conversionOk);

    float prix_r = 0;

    Budget B(id, prix_e, prix_r);
    bool test = B.ajouter();


    QMessageBox msgBox;

    if (test) {
        msgBox.setText("Ajout avec succès");
        ui->tableView->setModel(B.afficher(sort));
    } else {
        msgBox.setText("Failed");
    }

    msgBox.exec();
}

void exportToPdf(QTableView* tableView, QString totalCoast)
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save PDF", "", "PDF Files (*.pdf)");

    if (!filePath.isEmpty()) {
        QPrinter printer(QPrinter::PrinterResolution);

        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        printer.setOutputFileName(filePath);

        QPainter painter(&printer);

        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

        tableView->render(&painter);

        QFont font = painter.font();
        font.setPointSize(20);
        painter.setFont(font);
        painter.drawText(20, printer.height() - 20, "Total Coast: " + totalCoast);

        QMessageBox msgBox;


       msgBox.setText("PDF exported successfully to: "+filePath);
       msgBox.exec();
    }
}


void MainWindow::on_pushButton_18_clicked()
{
    QString totalCoast = ui->totalCostLabel->text();
    exportToPdf(ui->tableView_buy, totalCoast);
}

void MainWindow::on_comboBox_2_currentTextChanged(const QString &text)
{
    if (text.isEmpty()or text=="Status :") {
        ui->tableView->setModel(B.afficher(sort));
    } else {
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM BUDGET WHERE STATUS = '" + text + "'");
        ui->tableView->setModel(model);
    }
}

