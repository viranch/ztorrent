#include "searchengine.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>

SearchEngine::SearchEngine(QObject *parent) :
    QObject(parent),
    m_manager(new QNetworkAccessManager(this))
{
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseReply(QNetworkReply*)));
}

SearchEngine::~SearchEngine()
{
    delete m_manager;
}

void SearchEngine::search(QString searchString)
{
    QUrl searchUrl = QUrl("http://torrentz.in/feed?q="+searchString);
    m_manager->get(QNetworkRequest(searchUrl));
}

void SearchEngine::parseReply(QNetworkReply *reply)
{
    QDomDocument doc;
    doc.setContent(reply->readAll());
    QDomElement root = doc.documentElement();
    QDomNodeList domItems = root.elementsByTagName("item");
    QList<Torrent> torrents;

    for(int i=0; i<domItems.size(); i++) {
        QDomElement domItem = domItems.at(i).toElement();
        torrents << Torrent(domItem);
    }

    emit finished(torrents);
}
