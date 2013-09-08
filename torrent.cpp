#include "torrent.h"

#include <QRegExp>

Torrent::Torrent(QDomElement elem) :
    QDomElement(elem)
{
    QString desc = property("description");
    QRegExp rx("Size: (.*) Seeds: (.*) Peers: (.*) Hash: (.*)");
    rx.indexIn(desc);

    addChild("size", rx.cap(1));
    addChild("seeds", rx.cap(2));
    addChild("peers", rx.cap(3));
    addChild("hash", rx.cap(4).toUpper());
}

void Torrent::addChild(QString tagName, QString text) {
    QDomDocument doc;

    QDomElement childNode = doc.createElement(tagName);
    QDomText textNode = doc.createTextNode(text);
    childNode.appendChild(textNode);
    appendChild(childNode);
}

QString Torrent::torcacheUrl(QString hash)
{
    return QString("http://torcache.net/torrent/%1.torrent").arg(hash.toUpper());
}

QString Torrent::searchUrl(QString hash)
{
    return QString("http://torrentz.com/%1").arg(hash.toLower());
}
