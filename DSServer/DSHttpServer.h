#ifndef DSHTTPSERVER_H
#define DSHTTPSERVER_H

#include <Tufao/HttpServer>
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

using namespace Tufao;

class DSHttpServer : public HttpServer
{
    Q_OBJECT
public:
    explicit DSHttpServer(QObject *parent = 0);

    void post(HttpServerRequest& request, HttpServerResponse& response);
    void get(HttpServerRequest& request, HttpServerResponse& response);

    QString rootDir();

signals:

public slots:
};

#endif // DSHTTPSERVER_H
