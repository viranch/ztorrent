#-------------------------------------------------
#
# Project created by QtCreator 2013-08-22T02:43:40
#
#-------------------------------------------------

QT       += core gui widgets network xml

TARGET = ztorrent
TEMPLATE = app


SOURCES +=\
        ztorrent.cpp \
    searchengine.cpp \
    main.cpp \
    torrent.cpp \
    transmission.cpp \
    settings.cpp \
    trdialog.cpp

HEADERS  += ztorrent.h \
    searchengine.h \
    torrent.h \
    transmission.h \
    trbackend.h \
    settings.h \
    trdialog.h

RESOURCES += \
    resources.qrc

FORMS    += ztorrent.ui \
    settings.ui \
    trdialog.ui

QMAKE_INFO_PLIST = Info.plist
