#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <QObject>
#include <QNetworkAccessManager>

class FeedItem;

class SearchEngine : public QObject
{
    Q_OBJECT
public:
    explicit SearchEngine(QObject *parent = 0);
    void search(QString searchString);
    
signals:
    void finished(QList<FeedItem*> feeds);
    
private slots:
    void parseReply(QNetworkReply* reply);

private:
    QNetworkAccessManager *m_manager;
    
};

#endif // SEARCHENGINE_H
