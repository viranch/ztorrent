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

    QString hash = rx.cap(4);
    addChild("hash", hash);
    addChild("torcacheUrl", "http://torcache.net/torrent/"+hash.toUpper()+".torrent");
    addChild("torrentzUrl", "http://torrentz.in/"+hash.toLower());
}

void Torrent::addChild(QString tagName, QString text) {
    QDomDocument doc;

    QDomElement childNode = doc.createElement(tagName);
    QDomText textNode = doc.createTextNode(text);
    childNode.appendChild(textNode);
    appendChild(childNode);
}
