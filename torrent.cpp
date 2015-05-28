#include "torrent.h"

#include <QRegExp>
#include <QDateTime>
#include <QStringList>

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
    addChild("last_added", chronicDuration(time_added.secsTo(QDateTime::currentDateTimeUtc())) + " ago");
}

void Torrent::addChild(QString tagName, QString text)
{
    QDomDocument doc;

    QDomElement childNode = doc.createElement(tagName);
    QDomText textNode = doc.createTextNode(text);
    childNode.appendChild(textNode);
    appendChild(childNode);
}

QString Torrent::chronicDuration(qint64 secs)
{
    uint years=0, months=0, days=0, hours=0, minutes=0;

    minutes = secs/60;
    hours = minutes/60; minutes %= 60;
    days = hours/24; hours %= 24;
    months = days/30; days %= 30;
    years = months/12; months %= 12;

    QStringList nums;
    if (years > 0) {
        nums << QString::number(years) + "y";
        if (months > 0) { nums << QString::number(months) + "mo"; }
    } else if (months > 0) {
        nums << QString::number(months) + "mo";
        if (days > 0) { nums << QString::number(days) + "d"; }
    } else if (days > 0) {
        nums << QString::number(days) + "d";
        if (hours > 0) { nums << QString::number(hours) + "h"; }
    } else if (hours > 0) {
        nums << QString::number(hours) + "h";
        if (minutes > 0) { nums << QString::number(minutes) + "m"; }
    } else if (minutes > 0) {
        nums << QString::number(minutes) + "m";
        if (secs > 0) { nums << QString::number(secs) + "s"; }
    } else if (secs > 0) {
        nums << QString::number(secs) + "s";
    } else {
        nums << "now";
    }

    return nums.join(' ');
}
