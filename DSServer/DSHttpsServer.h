#ifndef DSHTTPSSERVER_H
#define DSHTTPSSERVER_H

#include <Tufao/HttpsServer>
#include <Tufao/HttpServerRequest>
#include <Tufao/HttpServerResponse>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QUrl>
#include "../DSDef.h"
#include "../DSUtil.h"
#include <QDir>
#include <QFile>
#include <Tufao/Headers>
#include "DSDBConnector.h"

using namespace Tufao;

class DSHttpsServer : public HttpsServer
{
    Q_OBJECT
public:
    explicit DSHttpsServer(QJsonObject& config, QObject *parent = 0);

    QJsonObject& _config;

    QJsonObject Login(QJsonObject request);
    QJsonObject Register(QJsonObject request);
    QJsonObject CheckSession(QJsonObject request);

    void httpResponse(HttpServerResponse& response, QJsonObject responseJson);

    void get(HttpServerRequest& request, HttpServerResponse& response);
    void post(HttpServerRequest& request, HttpServerResponse& response);



    QString md5(QString password);

signals:

public slots:
};

#endif // DSHTTPSSERVER_H
