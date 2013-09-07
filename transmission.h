#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "trbackend.h"

class Transmission : public QObject
{
    Q_OBJECT
public:
    explicit Transmission(QObject *parent = 0);

    void addTorrent(QString url, TrBackend backend);

signals:
    void finished(QString result, QString name);

public slots:

private slots:
    void parseResponse(QNetworkReply *reply);
    void authenticate(QNetworkReply *reply, QAuthenticator *auth);

private:
    QNetworkAccessManager *m_manager;
    QByteArray m_sessionId;

};

#endif // TRANSMISSION_H
