#include "DSHttpServer.h"
#include <QCoreApplication>
DSHttpServer::DSHttpServer(QObject *parent) : HttpServer(parent)
{
    connect(this, &DSHttpServer::requestReady, [&](HttpServerRequest &request, HttpServerResponse &response)
    {
        if(request.method() != "POST")
        {
            this->get(request, response);
        }

        connect(&request, &HttpServerRequest::end, [&]()
        {
            this->post(request, response);
        });
    });

    if(this->listen(QHostAddress::Any, DS_PORT_HTTP))
    {
        qDebug() << "http listen " << DS_PORT_HTTP << "OK";
    }
    else
    {
        qDebug() << "listen error";
        QCoreApplication::instance()->quit();
    }
}

void DSHttpServer::post(HttpServerRequest &request, HttpServerResponse &response)
{

}

void DSHttpServer::get(HttpServerRequest &request, HttpServerResponse &response)
{
    QString urlPath = request.url().path();
    QString filePath = rootDir() + urlPath;

    QFile file(filePath);
    qDebug() << filePath;
    if(!file.open(QFile::ReadOnly))
    {
        response.writeHead(HttpResponseStatus::NOT_FOUND);
        response.end("NOT FOUND");
        return;
    }

    response.writeHead(HttpResponseStatus::OK);
    response.headers().replace("Content-Type", "text/html");
    response.end(file.readAll());

    file.close();
}

QString DSHttpServer::rootDir()
{
    return QDir::homePath() + "/ds-html";
}
