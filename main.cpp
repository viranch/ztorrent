#include "ztorrent.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Viranch, Inc.");
    QCoreApplication::setApplicationName("ZTorrent");
    ZTorrent w;
    w.show();

    return a.exec();
}

