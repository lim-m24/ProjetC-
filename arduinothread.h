// In ArduinoThread.h
#ifndef ARDUINOTHREAD_H
#define ARDUINOTHREAD_H

#include <QThread>
#include <QSerialPort>
#include <QDebug>
#include <QSqlQuery>
#include <QDate>
#include <QSqlError>
#include <QSerialPortInfo>

class ArduinoThread : public QThread
{
    Q_OBJECT

public:
    ArduinoThread(QObject *parent = nullptr);
signals:
    void nbPValueChanged(int value);

protected:
    void run() override;

private:
    QSerialPort *serial;

    void openSerialPort();
    void closeSerialPort();
    void updateDatabase();
};

#endif // ARDUINOTHREAD_H
