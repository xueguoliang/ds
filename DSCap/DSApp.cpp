#include "DSApp.h"
#include "../DSDef.h"
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QCoreApplication>

DSApp::DSApp(QObject *parent) : QObject(parent)
{
    av_register_all();
    avdevice_register_all();

    connect(&this->_sock, SIGNAL(readyRead()),
            this, SLOT(slotReadyRead()));
    if(!_sock.bind(QHostAddress::Any, DS_PORT_CAP))
    {
        QCoreApplication::instance()->quit();
        return;
    }

    this->_cap = NULL;
}

void DSApp::checkSession(QString session)
{
    if(_cap && _cap->_checked)
        return;

    _man = new QNetworkAccessManager;
    connect(_man, &QNetworkAccessManager::finished, [&](QNetworkReply* reply){
        if(reply->error() == QNetworkReply::NoError)
        {
            QByteArray buf = reply->readAll();
            QJsonObject obj = QJsonDocument::fromJson(buf).object();
            if(_cap)
                _cap->_checked = obj.value(DS_RESULT).toString() == DS_OK;

            if(obj.value(DS_RESULT).toString() != DS_OK)
            {
                QJsonObject obj;
                obj.insert(DS_CMD, DS_STOP);
                obj.insert(DS_REASON, "账户已经在其它电脑登陆，你已经被踢出");
                QByteArray buf = QJsonDocument(obj).toJson();
                this->_sock.writeDatagram(buf, QHostAddress::LocalHost, DS_PORT_DS);
            }
        }
        else
        {
            QJsonObject obj;
            obj.insert(DS_CMD, DS_STOP);
            obj.insert(DS_REASON, "验证失败");
            QByteArray buf = QJsonDocument(obj).toJson();
            this->_sock.writeDatagram(buf, QHostAddress::LocalHost, DS_PORT_DS);
        }
        _man->deleteLater();
    });
    QJsonObject obj;
    obj.insert(DS_CMD, DS_CHECKSESSION);
    obj.insert(DS_SESSION, session);

    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_0);

    QString url=QString("https://www.ds-project.cn:%1/user").arg(DS_PORT_HTTPS);
    QNetworkRequest req;
    req.setSslConfiguration(config);
    req.setUrl(QUrl(url));
    _man->post(req, QJsonDocument(obj).toJson());
}

/* this function run in main thread */
void DSApp::slotReadyRead()
{
    while(_sock.hasPendingDatagrams())
    {
        quint64 len = _sock.pendingDatagramSize();
        QByteArray buf(len, 0);

        _sock.readDatagram(buf.data(), buf.size());

        QJsonDocument doc = QJsonDocument::fromJson(buf);
        QJsonObject obj = doc.object();

        if(obj.value(DS_CMD) == DS_START)
        {
            int interval = 100;
            if(!obj.value(DS_INTERVAL).isNull())
                interval = obj.value(DS_INTERVAL).toInt();
            if(interval < 100 || interval > 2000)
                interval = 100;

            int scale = 100;
            if(!obj.value(DS_SCALE).isNull())
                scale = obj.value(DS_SCALE).toInt();
            if(scale > 100 || scale < 0)
                scale = 100;

            int w = obj.value(DS_WIDTH).toInt();
            int h = obj.value(DS_HEIGHT).toInt();

            if(this->_cap)
                stop();
            QThread::msleep(100);

            start(interval, scale, w, h);
            checkSession(obj.value(DS_SESSION).toString());
        }
        else if(obj.value(DS_CMD) == DS_STOP)
        {
            stop();
        }
    }
}

void DSApp::start(int interval, int scale, int width, int height)
{
    this->_cap = new DSCap;
    this->_cap->_sock = &_sock;
    this->_cap->start(interval, scale, width, height);
}

void DSApp::stop()
{
    if(_cap)
    {
        _cap->stop();
        _cap = NULL;
    }
}


