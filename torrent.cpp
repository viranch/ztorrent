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
    QString filename = property("title").replace(' ', '.').toLower();
    addChild("torcache", "http://torcache.net/torrent/"+property("hash")+".torrent?title="+filename);
}

void Torrent::addChild(QString tagName, QString text) {
    QDomDocument doc;

    QDomElement childNode = doc.createElement(tagName);
    QDomText textNode = doc.createTextNode(text);
    childNode.appendChild(textNode);
    appendChild(childNode);
}
