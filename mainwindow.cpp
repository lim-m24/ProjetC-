#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "forniseur.h"
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDoubleValidator>
#include <QDebug>

#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QFileDialog>

#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarCategoryAxis>


using namespace QtCharts;
int sort,text;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sort=0;
    text = ui->combo_id->currentText().toInt();
    ui->tableView->setModel(F.afficher(sort));
    ui->tableView_2->setModel(F.afficher2(text));
    ui->le_id->setValidator(new QIntValidator(1,99999999,this));
    ui->le_id_2->setValidator(new QIntValidator(1,99999999,this));
    ui->le_id_3->setValidator(new QIntValidator(1,99999999,this));
    ui->le_tel->setValidator(new QIntValidator(10000000,99999999,this));
    ui->le_tel_2->setValidator(new QIntValidator(10000000,99999999,this));

    ///////////////////combo id //////////////////////////
    QStringList items;

     QSqlQuery query,query2;
     query.exec("SELECT ID FROM FORNISUER");

     while (query.next()) {
         QString item = query.value(0).toString();
         items.append(item);
     }

     ui->combo_id->clear();
     ui->combo_id->addItems(items);

     connect(ui->combo_id, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(updateNomLabel(const QString&)));

     connect(ui->combo_id, &QComboBox::currentTextChanged, this, &MainWindow::updateNomLabel);
     connect(ui->combo_id, &QComboBox::currentTextChanged, this, &MainWindow::tempSlot);

     ////////////////
     ///////////////////STAT////////////////////
     QSqlQuery query5("SELECT ID_F, ID_P, PRIX, QTE FROM DISTRIBUTION");

     // Create sets for each product
     QBarSet *setProduct1 = new QBarSet("Product 1");
     QBarSet *setProduct2 = new QBarSet("Product 2");
     QBarSet *setProduct3 = new QBarSet("Product 3");
     QBarSet *setProduct4 = new QBarSet("Product 4");
     QBarSet *setProduct5 = new QBarSet("Product 5");
     QBarSet *setProduct6 = new QBarSet("Product 6");


     QBarCategoryAxis *axis = new QBarCategoryAxis();

     while (query5.next()) {
         int idProduct = query5.value("ID_P").toInt();
         float price = query5.value("PRIX").toFloat();

         switch (idProduct) {
             case 1:
                 setProduct1->append(price);
                 break;
             case 2:
                 setProduct2->append(price);
                 break;
             case 3:
                 setProduct3->append(price);
                 break;
             case 4:
                 setProduct4->append(price);
                 break;
             case 5:
                 setProduct5->append(price);
                 break;
             case 6:
                 setProduct6->append(price);
                 break;
             default:
                 break;
         }
     }

     QBarSeries *series = new QBarSeries();
     series->append(setProduct1);
     series->append(setProduct2);
     series->append(setProduct3);
     series->append(setProduct4);
     series->append(setProduct5);
     series->append(setProduct6);

     QChart *chart = new QChart();
     chart->addSeries(series);
     chart->setTitle("Price Differences Between Suppliers and Products");
     chart->setAnimationOptions(QChart::SeriesAnimations);

     axis->setTitleText("Supplier");
     chart->createDefaultAxes();
     chart->setAxisX(axis, series);

     QChartView *chartView = new QChartView(chart);
     chartView->setParent(ui->horizontalFrame);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tempSlot(const QString &text) {
    qDebug() << "Combo box value changed:" << text;
}
void MainWindow::on_ajouter_clicked()
{
    int id = ui->le_id->text().toInt();
    QString nom = ui->le_nom->text();
    QString email = ui->le_email->text();
    int tel = ui->le_tel->text().toInt();
    QString adresse = ui->le_adresse->text();

    Forniseur F(id, nom, email,tel,adresse);
    bool test = F.ajouter();
    QMessageBox msgBox;

    if (test) {
        msgBox.setText("Ajout avec succès");
        ui->tableView->setModel(F.afficher(sort));
    } else {
        msgBox.setText("Failed");
    }

    msgBox.exec();
}

void MainWindow::on_supprimer_clicked()
{
    Forniseur F1; F1.setid(ui->le_id_3->text().toInt());
    bool test=F1.supprimer(F1.getid());
    QMessageBox msgBox;
    if(test){
        msgBox.setText("Suppression avec succes");
        ui->tableView->setModel(F.afficher(sort));
    }
    else
        msgBox.setText("failed");
        msgBox.exec();
}


void MainWindow::on_Modifier_clicked()
{
    int id = ui->le_id_2->text().toInt();
    QString nom = ui->le_nom_2->text();
    QString email = ui->le_email_2->text();
    int tel = ui->le_tel_2->text().toInt();
    QString adresse = ui->le_adresse_2->text();

    Forniseur F(id, nom, email,tel,adresse);
    bool test = F.modifier();
    QMessageBox msgBox;

    if (test) {
        msgBox.setText("Update avec succès");
        ui->tableView->setModel(F.afficher(sort));
    } else {
        msgBox.setText("Failed");
    }

    msgBox.exec();
}

void MainWindow::on_pushButton_clicked()
{
    QString selectedValue = ui->comboBox->currentText();
    if(selectedValue=="ID"){
        if(sort==1){
            sort=0;
        }else{
            sort=1;
        }
    }else{
        if(sort==3){
            sort=2;
        }else{
            sort=3;
        }
    }
    ui->tableView->setModel(F.afficher(sort));

}

void MainWindow::on_lineEdit_textChanged(const QString &text)
{
    if (text.isEmpty()) {
        ui->tableView->setModel(F.afficher(sort));
    } else {
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM FORNISUER WHERE ID LIKE '" + text + "%' or NOM LIKE '" + text + "%' or EMAIL LIKE '" + text + "%' or TEL LIKE '" + text + "%'");
        ui->tableView->setModel(model);
    }
}

void MainWindow::updateNomLabel(const QString &text) {
    qDebug() << "updateNomLabel called with text:" << text;

    QSqlQuery query2;
    query2.prepare("SELECT NOM FROM FORNISUER WHERE ID = :selectedValue");
    query2.bindValue(":selectedValue", text);

    if (query2.exec() && query2.next()) {
        QString idd = query2.value(0).toString();
        ui->labe_nom->setText(idd);

        ui->tableView_2->setModel(F.afficher2(text.toInt()));
        qDebug() << "Row count in tableView_2 model:" << ui->tableView_2->model()->rowCount();

    }
}


void MainWindow::on_pushButton_2_clicked()
{
    int selectedRadio =0;
    int selectedQte =0;
    float price;
    text = ui->combo_id->currentText().toInt();

    if (ui->radioButton->isChecked()) {
        selectedRadio = 1;
    } else if (ui->radioButton_2->isChecked()) {
        selectedRadio = 2;
    } else if (ui->radioButton_3->isChecked()) {
        selectedRadio = 3;
    }else if (ui->radioButton_4->isChecked()) {
        selectedRadio = 4;
    }else if (ui->radioButton_5->isChecked()) {
        selectedRadio = 5;
    }else if (ui->radioButton_6->isChecked()) {
        selectedRadio = 6;
    }
    selectedQte = ui->spinBox->value();
    price= ui->lineEdit_2->text().toFloat();

    if (selectedRadio && selectedQte > -1) {
        QSqlQuery query;
        QMessageBox msgBox;

        query.prepare("INSERT INTO DISTRIBUTION (ID_F, ID_P, PRIX, QTE) VALUES (:ID_F, :ID_P, :PRIX, :QTE)");
        QString idString = QString::number(text);
        QString selectedRadio1 = QString::number(selectedRadio);
        QString price1 = QString::number(price);
        QString selectedQte1 = QString::number(selectedQte);

        query.bindValue(":ID_F", idString);
        query.bindValue(":ID_P", selectedRadio1);
        query.bindValue(":PRIX", price1);
        query.bindValue(":QTE", selectedQte1);

        if (query.exec()) {
            msgBox.setText("Ajout avec succès");
            ui->tableView_2->setModel(F.afficher2(text));
        }
    }
}

void exportToPdf(QTableView* tableView_2)
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

        tableView_2->render(&painter);

        QFont font = painter.font();
        font.setPointSize(20);
        painter.setFont(font);


        QMessageBox msgBox;


       msgBox.setText("PDF exported successfully to: "+filePath);
       msgBox.exec();
    }
}
void MainWindow::on_pushButton_3_clicked()
{
    exportToPdf(ui->tableView_2);
}
