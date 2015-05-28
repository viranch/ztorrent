#include "torrent.h"

#include <QRegExp>
#include <QDateTime>

Torrent::Torrent(QDomElement elem) :
    QDomElement(elem)
{
    QString desc = property("description");
    QRegExp rx("Size: (.*) Seeds: (.*) Peers: (.*) Hash: (.*)");
    rx.indexIn(desc);

    addChild("size", rx.cap(1));
    addChild("seeds", rx.cap(2));
    addChild("peers", rx.cap(3));

    QString hash = rx.cap(4);
    addChild("hash", hash);
    addChild("torcacheUrl", "http://torcache.net/torrent/"+hash.toUpper()+".torrent");
    addChild("torrentzUrl", "http://torrentz.in/"+hash.toLower());

    QDateTime time_added = QDateTime::fromString(property("pubDate"), "ddd, dd MMM yyyy hh:mm:ss +0000");
    time_added.setTimeSpec(Qt::UTC);
    qint64 last_added_days = time_added.daysTo(QDateTime::currentDateTimeUtc());
    if (last_added_days == 0) {
        addChild("last_added", QString::number(time_added.secsTo(QDateTime::currentDateTimeUtc()) / (60*60)) + " hours ago");
    } else {
        addChild("last_added", QString::number(last_added_days) + " days ago");
    }
}

void Torrent::addChild(QString tagName, QString text)
{
    QDomDocument doc;

    QDomElement childNode = doc.createElement(tagName);
    QDomText textNode = doc.createTextNode(text);
    childNode.appendChild(textNode);
    appendChild(childNode);
}
