#ifndef DSRECV_H
#define DSRECV_H

#include <QThread>
#include <QImage>
#include <QUdpSocket>
#include <QMap>
#include <QImage>
#include <QTcpSocket>
#include "../DSUtil.h"

class DSRecv : public QObject
{
    Q_OBJECT
public:
    explicit DSRecv(QObject *parent = 0);
    QUdpSocket* sock;
    QThread* thread;

    QMap<quint32, QMap<quint16, QByteArray> > packets;
    QImage image;
    void combine(quint32 frameIdx);

    QHostAddress _addr;
signals:
    void sigImage(QImage image, int x, int y);

public slots:
    void slotReadyRead();
    void slotAsk(QByteArray buf);
};

#endif // DSRECV_H
