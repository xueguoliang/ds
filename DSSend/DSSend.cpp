#include "DSSend.h"
#include "../DSDef.h"
#include <QDataStream>

DSSend::DSSend(QObject *parent) : QObject(parent)
{
    sock = new QUdpSocket(this);
    sock->bind();
    thread = new QThread;
    this->moveToThread(thread);
    thread->start();
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

#ifndef Q_OS_LINUX
    qDebug() << "send buffer origin:" << sock->socketOption(QUdpSocket::SendBufferSizeSocketOption).toInt();

    int buflen = 1024*1024;
    while(buflen > 8*1024)
    {
        sock->setSocketOption(QUdpSocket::SendBufferSizeSocketOption, buflen);
        int len = sock->socketOption(QUdpSocket::SendBufferSizeSocketOption).toInt();
        qWarning() << "now len" << len;
        if(len == buflen) break;
        buflen /= 2;
    }
    #endif
    qDebug() << "send buffer setup:" << sock->socketOption(QUdpSocket::SendBufferSizeSocketOption).toInt();

}

void DSSend::slotBuffer(QByteArray buf, quint32 frameIdx)
{
    const int blocksize = 800;
    ushort total = buf.size()/blocksize;
    if(buf.size() %blocksize) total++;

    for(ushort index=1; index<=total; ++index)
    {
        QByteArray sub = buf.mid((index-1)*blocksize, blocksize);
        QByteArray packet;
        QDataStream ds(&packet, QIODevice::ReadWrite);
        ds << index << total << sub << frameIdx;

        sock->writeDatagram(packet, QHostAddress(shareaddr), DS_PORT_VIEW);
    }
}
