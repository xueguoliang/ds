#ifndef DSAPP_H
#define DSAPP_H

#include <QObject>
#include <QUdpSocket>
#include "../DSDef.h"
#include <QImage>
#include "DSSend.h"
#include "DSFrame.h"
#include "../DSUtil.h"
class DSApp : public QObject
{
    Q_OBJECT
public:
    explicit DSApp(QObject *parent = 0);
    ~DSApp();

    QUdpSocket _sock;
    void readDatagram();

    DSSend* send;
    DSFrame* frame;

    int _width;
    int _height;

    void attachMemory();
    void detachMemory();
    uchar* _shm = NULL;
signals:
    void sigNewFrame(QImage image, int x, int y);
    void sigShareAddrChanged(QString addr);
    void sigShareModeChanged(QString sharemode);

public slots:
};

#endif // DSAPP_H
