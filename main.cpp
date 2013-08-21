#include "ztorrent.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZTorrent w;
    w.show();

    return a.exec();
}

