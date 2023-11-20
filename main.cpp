#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include <cstdlib>
#include "login.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection c;
    bool test=c.createconnect();
    if(!test) {
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                    QObject::tr("connection failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
        exit(EXIT_FAILURE);
    }

    Login L;
    L.show();
    return a.exec();
}
