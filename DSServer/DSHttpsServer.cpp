#include "DSHttpsServer.h"
#include <QFile>
#include <QSslKey>
#include <QSslCertificate>
#include <QUrl>
#include <QHostAddress>

#include <QCryptographicHash>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QUuid>
#include <QDateTime>
#include <QCoreApplication>

DSHttpsServer::DSHttpsServer(QJsonObject& config, QObject *parent) :
    _config(config), HttpsServer(parent)
{
    QFile key(":/key.pem");
    key.open(QFile::ReadOnly);
    setPrivateKey(QSslKey(&key, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, "xxxxxx"));

    QFile cert(":/cert.pem");
    cert.open(QFile::ReadOnly);
    setLocalCertificate(QSslCertificate(&cert));

    connect(this, &HttpsServer::requestReady, [&](HttpServerRequest& request, HttpServerResponse& response)
    {
        if(request.method() != "POST")
        {
            get(request, response);
            return;
        }
        connect(&request, &HttpServerRequest::end, [&]()
        {
            post(request, response);
        });
    });

    if(listen(QHostAddress::Any, DS_PORT_HTTPS))
    {
        qDebug() << "listen ok" << DS_PORT_HTTPS;
    }
    else
    {
        QCoreApplication::instance()->quit();

    }

}

void DSHttpsServer::post(HttpServerRequest& request, HttpServerResponse& response)
{
    QJsonObject requestJson;
    {
        QByteArray buf = request.readBody();
        QJsonDocument doc = QJsonDocument::fromJson(buf);
        requestJson = doc.object();
    }
    qWarning() << requestJson;
    QJsonObject responseJson;
    {
        responseJson.insert(DS_RESULT, QString(DS_ERR));
        responseJson.insert(DS_REASON, QString("unknown error"));
    }

    QString cmd = requestJson.value(DS_CMD).toString();
    if(cmd == QString(DS_LOGIN))
    {
        responseJson = Login(requestJson);
    }
    else if(cmd == QString(DS_REGISTER))
    {
        responseJson = Register(requestJson);
    }
    else if(cmd == QString(DS_CHECKSESSION))
    {
        responseJson = CheckSession(requestJson);
    }

    httpResponse(response, responseJson);
}


QJsonObject DSHttpsServer::Login(QJsonObject request)
{
    QJsonObject response;
    response.insert(DS_RESULT, QString(DS_ERR));
    response.insert(DS_ADURL, _config.value(DS_ADURL));

    // auto disconnect
    DSDBConnector db(_config);

    QString username = request.value(DS_USERNAME).toString();
    QString password = request.value(DS_PASSWORD).toString();
    password = md5(password);
    QString sessionid = md5(QUuid::createUuid().toString());
    QString app = request.value(DS_APP).toString();

    {
        QString sql = QString("select * from tuser where f2='%1'  and f3='%2'").arg(username, password);
        QSqlQuery query;
        if(!query.exec(sql))
        {
            response.insert(DS_RESULT, QString(DS_ERR));
            response.insert(DS_REASON, query.lastError().text());
            return response;
        }

        if(!query.first())
        {
            response.insert(DS_RESULT, QString(DS_ERR));
            response.insert(DS_REASON, QString("username or password error"));
            return response;
        }
    }

    {

        QString sql = QString("update tuser set f4='%1' where f2='%2'").arg(sessionid, username);
        QSqlQuery query;
        if(!query.exec(sql))
        {
            response.insert(DS_RESULT, QString(DS_ERR));
            response.insert(DS_REASON, query.lastError().text());
            return response;
        }
    }

    // Login Log
    {
        QString sql = QString("insert into tloginlog (f1, f2, f3) values("
                              "'%1', '%2', '%3') ")
                .arg(username.left(16), app.left(16)).arg(QDateTime::currentMSecsSinceEpoch());
        QSqlQuery query;
        query.exec(sql);
    }

    response.insert(DS_RESULT, QString(DS_OK));
    response.insert(DS_SESSION, sessionid);

    return response;
}

QJsonObject DSHttpsServer::Register(QJsonObject request)
{
    qDebug() << "call register";

    QJsonObject response;
    QString username = request.value(DS_USERNAME).toString();
    QString password = request.value(DS_PASSWORD).toString();
    QString qq = request.value(DS_QQ).toString();
    QString mobile = request.value(DS_MOBILE).toString();
    password = md5(password);

    QString sessionid = md5(QUuid::createUuid().toString());
    QString verify = md5(QUuid::createUuid().toString());

    // auto disconnector
    DSDBConnector db(_config);

    QString sql = QString("insert into tuser (f2, f3, f4, f5, f6, f7) values("
                          "'%1', '%2', '%3', '%4', '%5', '%6') ")
            .arg(username, password, sessionid, qq, mobile, verify);

    QSqlQuery query;
    if(query.exec(sql))
    {
        // success
        response.insert(DS_RESULT, QString(DS_OK));
        response.insert(DS_VERIFY, verify);
    }
    else
    {
        response.insert(DS_RESULT, QString(DS_ERR));
        response.insert(DS_REASON, query.lastError().text());
    }

    return response;
}

QJsonObject DSHttpsServer::CheckSession(QJsonObject request)
{
    QJsonObject response;
    QString sessionid = request.value(DS_SESSION).toString();

    // auto disconnector
    DSDBConnector db(_config);

    QString sql = QString("select * from tuser where f4='%1'").arg(sessionid);

    QSqlQuery query;

    if(query.exec(sql) && query.first())
    {
        response.insert(DS_RESULT, QString(DS_OK));
        return response;
    }

    response.insert(DS_RESULT, QString(DS_ERR));
    return response;
}

void DSHttpsServer::httpResponse(HttpServerResponse& response, QJsonObject responseJson)
{
    QJsonDocument doc(responseJson);
    QByteArray buf = doc.toJson();

    response.writeHead(HttpResponseStatus::OK);
    response.end(buf);
}

void DSHttpsServer::get(HttpServerRequest &request, HttpServerResponse &response)
{

}

QString DSHttpsServer::md5(QString password)
{
    QByteArray buf = QCryptographicHash::hash(password.toUtf8(),
                                              QCryptographicHash::Md5).toHex();
    buf = buf.mid(3, 8) + buf.right(8);
    return buf;
}


