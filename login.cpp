#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

QSerialPort *serial;
Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    // Initialize and open the Serial port
    serial = new QSerialPort(this);
    serial->setPortName("COM4"); // Replace "COMx" with your Arduino's port
    serial->setBaudRate(QSerialPort::Baud9600);
    if (serial->open(QIODevice::ReadWrite)) {
        connect(serial, &QSerialPort::readyRead, this, &Login::readData);
    } else {
        QMessageBox::critical(this, "Error", "Could not open Serial port.");
    }
}

Login::~Login()
{
    if (serial->isOpen()) {
        serial->close();
    }
    delete ui;
}

void Login::on_pushButton_Login_clicked()
{
    // Regular login using username and password
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    QString username1 = ui->lineEdit_username->text();
    QString password1 = ui->lineEdit_password->text();

    QSqlQuery query, query2;
    query.prepare("SELECT ID_E FROM EMPLOYE WHERE USERNAME = :username AND PASSWORD = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        QString idd = query.value(0).toString();
        query2.prepare("SELECT NOM,PRENOM FROM EMPLOYE WHERE ID_E = :idd");
        query2.bindValue(":idd", idd);

        if (query2.exec() && query2.next()) {
            QString nom = query2.value(0).toString();
            QString prenom = query2.value(1).toString();
            QMessageBox::information(this, "Login", "Welcome " + nom + " " + prenom);
            username1=nom;
            password1=prenom;
        }
        hide();
        serial->close();
        mainwindow = new MainWindow(this);

        mainwindow->show();
    } else {
        QMessageBox::warning(this, "Login", "Username and password are not correct");
    }
}

void Login::handleRFIDScan(QByteArray data)
{
    QString code = data;
    qDebug() << "Received data: " << code;

    QSqlQuery query;
    query.prepare("SELECT EMPLOYE.USERNAME, EMPLOYE.PASSWORD FROM EMPLOYE INNER JOIN tags ON EMPLOYE.ID_E = tags.ID_E WHERE tags.id_tag = :idTag");
    query.bindValue(":idTag", code);

    if (query.exec() && query.next()) {
        QString username = query.value("USERNAME").toString();
        QString password = query.value("PASSWORD").toString();
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
        // Fill the line edits with the retrieved username and password
        ui->lineEdit_username->setText(username);
        ui->lineEdit_password->setText(password);

        //QMessageBox::information(this, "RFID Scan", "RFID tag recognized.");
    } else {
        QMessageBox::warning(this, "RFID Scan", "RFID tag not recognized");
    }

}
void Login::readData() {
    if (serial->isOpen()) {
        buffer.append(serial->readAll());

        while (buffer.contains('\n')) {
            QByteArray data = buffer.left(buffer.indexOf('\n')).trimmed();
            buffer.remove(0, buffer.indexOf('\n') + 1);

            if (!data.isEmpty()) {
                qDebug() << "Received data: " << data;

                if (data !="0" ) {
                    // Handle the RFID scan
                    handleRFIDScan(data);
                }
            }
        }
    } else {
        qDebug() << "Serial port is not open";
    }

}



