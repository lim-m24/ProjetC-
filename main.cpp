#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include <QDebug>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include "arduinothread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Connect to the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("PROJETCPP");
    db.setUserName("aziz");
    db.setPassword("aziz");

    if (!db.open()) {
        qDebug() << "Erreur de connexion à la base de données : " << db.lastError().text();
        return -1;
    }
    QSerialPort serial;
    serial.setPortName("COM4");
    if (!serial.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open serial port:" << serial.errorString();
        return -1;
    }


    Connection c;
    bool test = c.createConnection();

    if (!test) {
        QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                              QObject::tr("Connection failed.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
        return -1;
    }

    MainWindow w;
    w.show();
    ArduinoThread arduinoThread(&serial);
    arduinoThread.start();

    return a.exec();
}
