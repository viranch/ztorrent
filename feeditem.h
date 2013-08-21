#ifndef FEEDITEM_H
#define FEEDITEM_H

#include <QDomElement>
#include <QString>

class FeedItem : public QDomElement
{
public:
    FeedItem(QDomElement elem);

    inline QString property(QString tag) { return firstChildElement(tag).text(); }
};

#endif // FEEDITEM_H
