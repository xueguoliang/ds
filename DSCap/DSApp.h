#ifndef DSAPP_H
#define DSAPP_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QHostAddress>
#include "DSCap.h"
class DSApp : public QObject
{
    Q_OBJECT
public:
    explicit DSApp(QObject *parent = 0);

    QUdpSocket _sock;
    DSCap* _cap;
    void checkSession(QString session);

    QNetworkAccessManager* _man;

signals:

public slots:
    void slotReadyRead();
    void start(int interval, int scale, int width, int height);
    void stop();
};

#endif // DSAPP_H
