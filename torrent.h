#ifndef TORRENT_H
#define TORRENT_H

#include <QDomElement>
#include <QString>
#include <QMetaType>

class Torrent : public QDomElement
{
public:
    Torrent(QDomElement elem = QDomElement());

    inline QString property(QString tag) { return firstChildElement(tag).text(); }

    inline QString operator[](const QString& key) { return property(key); }

private:
    void addChild(QString tagName, QString text);
};

Q_DECLARE_METATYPE(Torrent)

#endif // TORRENT_H
