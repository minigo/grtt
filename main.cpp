#include "MainDialog.h"
#include <QApplication>

int main (int argc, char *argv[])
{
    QApplication a (argc, argv);

    QApplication::setOrganizationName ("MySoft");
    QApplication::setOrganizationDomain ("crazycoding.xyz");
    QApplication::setApplicationName ("Qt Redmine Time Tracker");

    MainDialog w;
    w.show ();
    return a.exec ();
}
