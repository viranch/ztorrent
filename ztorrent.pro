#-------------------------------------------------
#
# Project created by QtCreator 2013-08-22T02:43:40
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ztorrent
TEMPLATE = app


SOURCES +=\
        ztorrent.cpp \
    searchengine.cpp \
    main.cpp \
    torrent.cpp

HEADERS  += ztorrent.h \
    searchengine.h \
    torrent.h

FORMS    += ztorrent.ui
