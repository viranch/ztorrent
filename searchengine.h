#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <QObject>
#include <QNetworkAccessManager>

#include "torrent.h"

class SearchEngine : public QObject
{
    Q_OBJECT
public:
    explicit SearchEngine(QObject *parent = 0);
    ~SearchEngine();

    void search(QString searchString);

signals:
    void finished(QList<Torrent> torrents);

private slots:
    void parseReply(QNetworkReply* reply);

private:
    QNetworkAccessManager *m_manager;

};

#endif // SEARCHENGINE_H
