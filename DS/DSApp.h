#ifndef DSAPP_H
#define DSAPP_H

#include <QObject>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QUdpSocket>
#include <QPixmap>
#include <QMenu>
#include <QPainter>
#include <QAction>
#include "../DSDef.h"
#include <QJsonObject>
#include <QProcess>
#include "../DSUtil.h"
#include <QTcpServer>

class DSApp : public QObject
{
    Q_OBJECT
public:
    explicit DSApp(QObject *parent = 0);
    ~DSApp();

    QSystemTrayIcon* _tray;
    QUdpSocket* _sock;
    QPixmap _start;
    QPixmap _stop;
    QPixmap _iconStop;
    QMenu* _contextMenu;
    QAction* _actionStart;
    QAction* _actionStop;
//    static const int _iconWidth = 32;
//    static const int _iconHeight = 32;
    bool _started;
    QString _session;
    QString _adurl;
    QString _record;
    QSize _screenSize;

#ifdef Q_OS_WIN
    QProcess _cap;
    QProcess _send;
#endif
#ifdef Q_OS_LINUX
    QProcess _cap;
    QProcess _send;
#endif
    void initTray();
    void initSocket();

    int _logoTimer;
    void start();
    void stop();

    void timerEvent(QTimerEvent *event);

    QJsonObject _setup;

    bool isRetina();
    void sendSetup();
    void recvData();
signals:

public slots:
};

#endif // DSAPP_H
