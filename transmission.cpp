#include "transmission.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QAuthenticator>

#define TR_SESSION_HEADER "X-Transmission-Session-Id"

Transmission::Transmission(QObject *parent) :
    QObject(parent),
    m_manager(new QNetworkAccessManager(this))
{
    connect(m_manager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
            this, SLOT(authenticate(QNetworkReply*,QAuthenticator*)));
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseResponse(QNetworkReply*)));
}

void Transmission::addTorrent(QString url, TrBackend backend)
{
    QString host = backend["host"].toString();
    QString port = backend["port"].toString();
    QString path = backend["path"].toString();
    QUrl requestUrl = "http://"+host+":"+port+path;
    QNetworkRequest request(requestUrl);
    if (!m_sessionId.isNull()) {
        request.setRawHeader(TR_SESSION_HEADER, m_sessionId);
    }

    QVariantMap data, args;
    args["filename"] = url;
    data["method"] = "torrent-add";
    data["arguments"] = args;
    QJsonObject json = QJsonObject::fromVariantMap(data);
    request.setAttribute(QNetworkRequest::User, url);
    request.setAttribute(QNetworkRequest::UserMax, backend);

    m_manager->post(request, QJsonDocument(json).toJson());
}

void Transmission::authenticate(QNetworkReply *reply, QAuthenticator *auth)
{
    TrBackend b = reply->request().attribute(QNetworkRequest::UserMax).toMap();
    auth->setUser(b["user"].toString());
    auth->setPassword(b["passwd"].toString());
}

void Transmission::parseResponse(QNetworkReply *reply)
{
    int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (code == 409) {
        m_sessionId = reply->rawHeader(TR_SESSION_HEADER);
        QString url = reply->request().attribute(QNetworkRequest::User).toString();
        TrBackend backend = reply->request().attribute(QNetworkRequest::UserMax).toMap();
        addTorrent(url, backend);
    } else {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QVariantMap data = jsonDoc.object().toVariantMap();
        QString result = data["result"].toString();
        QString name;
        if (result == "success") {
            name = data["arguments"].toMap()["torrent-added"].toMap()["name"].toString();
        }
        emit finished(result, name);
    }
    reply->close();
    reply->deleteLater();
}
