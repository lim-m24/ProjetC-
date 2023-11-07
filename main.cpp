#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;

    if (c.createconnect())
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("Database is not open"));
        msgBox.setInformativeText(QObject::tr("Connection failed. Click OK to exit."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Ok){
            return 1;
        }
    }

    return a.exec();
}

