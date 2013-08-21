#include "searchengine.h"
#include "feeditem.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>

SearchEngine::SearchEngine(QObject *parent) :
    QObject(parent),
    m_manager(new QNetworkAccessManager(this))
{
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseReply(QNetworkReply*)));
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
    QList<FeedItem*> feeds;

    for(int i=0; i<domItems.size(); i++) {
        QDomElement domItem = domItems.at(i).toElement();
        feeds << new FeedItem(domItem);
    }

    emit finished(feeds);
}
