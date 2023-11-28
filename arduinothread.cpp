
#include "arduinothread.h"

ArduinoThread::ArduinoThread(QObject *parent)
    : QThread(parent), serial(nullptr)
{
}


void ArduinoThread::run()
{
    openSerialPort();

    QByteArray buffer;


    while (!isInterruptionRequested()) {
        if (serial->waitForReadyRead(1000)) {
            QByteArray data = serial->readAll();
            buffer += data;
            QList<QByteArray> lines = buffer.split('\n');
            for (int i = 0; i < lines.size() - 1; ++i) {
                QString message = QString::fromLatin1(lines.at(i)).trimmed();
                qDebug() << "Arduino Output:" << message;
                if (message == "1") {
                    updateDatabase();
                }
            }
            buffer = lines.last();
        }
    }

    closeSerialPort();
}

void ArduinoThread::openSerialPort()
{
    serial = new QSerialPort(this);
    serial->setPortName("COM4");

    if (!serial->open(QIODevice::ReadWrite)) {
        qWarning() << "Failed to open serial port:" << serial->errorString();
        delete serial;
        serial = nullptr;
    }
}

void ArduinoThread::closeSerialPort()
{
    if (serial && serial->isOpen()) {
        serial->close();
        delete serial;
        serial = nullptr;
    }
}

void ArduinoThread::updateDatabase()
{


    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Port: " << info.portName();
    }

    QSqlQuery query;


    QString currentDate = QDate::currentDate().toString("yyyy-MM-dd");
    QString checkQuery = "SELECT COUNT(*) FROM pointage WHERE DATE_P = :currentDate";
    query.prepare(checkQuery);
    query.bindValue(":currentDate", currentDate);

    if (!query.exec()) {
        qWarning() << "Failed to execute check query:" << query.lastError().text();
        return;
    }

    query.next();
    int rowCount = query.value(0).toInt();
    if (rowCount > 0) {
        qDebug() << "Row with current date exists";
        QString selectQuery = "SELECT NB_P FROM pointage WHERE DATE_P = :currentDate";
        query.prepare(selectQuery);
        query.bindValue(":currentDate", currentDate);

        if (query.exec() && query.next()) {
            int NB_P_value = query.value("NB_P").toInt();
            qDebug() << "Retrieved NB_P value:" << NB_P_value;
            qDebug() << "Sending NB_P value to Arduino:" << NB_P_value;
            QSerialPort serial;
            serial.setPortName("COM4");
            if (serial.isOpen() && serial.isWritable()) {
                QString datalcd = "Pointage " + QString::number(NB_P_value);
                QByteArray data = datalcd.toUtf8();
                serial.write(data);
                serial.flush();
            } else {
                qDebug() << "Serial port is not open or not writable";
            }
    } else {
        QString insertQuery = "INSERT INTO pointage (DATE_P, NB_P) VALUES (:currentDate, 0)";
        query.prepare(insertQuery);
        query.bindValue(":currentDate", currentDate);

        if (query.exec()) {
            qDebug() << "New row inserted successfully";
        } else {
            qWarning() << "Failed to insert new row:" << query.lastError().text();
        }
    }
    emit nbPValueChanged(42);
}
}
