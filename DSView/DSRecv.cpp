#include "DSRecv.h"
#include "../DSDef.h"
#include <QDataStream>
#include <QPainter>
DSRecv::DSRecv(QObject *parent) : QObject(parent)
{
    sock = new QUdpSocket(this);
    if(!sock->bind(QHostAddress::Any, DS_PORT_VIEW))
    {
        qWarning() << "bind error";
    }
    connect(sock, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

    thread = new QThread;
    moveToThread(thread);
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

#ifndef Q_OS_LINUX
    int buflen = 1024*1024;
    while(buflen > 8*1024)
    {
        sock->setSocketOption(QUdpSocket::ReceiveBufferSizeSocketOption, buflen);
        int len = sock->socketOption(QUdpSocket::ReceiveBufferSizeSocketOption).toInt();
        if(len == buflen) break;
        buflen /= 2;
    }
#endif
    qDebug() << "recv buffer:" << sock->socketOption(QUdpSocket::ReceiveBufferSizeSocketOption).toInt();
}

void DSRecv::combine(quint32 frameIdx)
{
    QMap<int, QByteArray> fragBufs;
    QString sharemode;
    int fragW;
    int fragH;
    int x, y;

    QByteArray buf;
    QMap<quint16, QByteArray>& framePackets = packets[frameIdx];
    for(quint16 idx = 1; idx <= framePackets.size(); ++idx)
    {
        if(framePackets.find(idx) == framePackets.end())
        {
            packets.remove(frameIdx);
            return;
        }
        buf += framePackets[idx];
    }

    char last = buf.at(buf.size()-1);
    buf = buf.mid(0, buf.size()-1);
    if(last == 'c')
    {
        buf = qUncompress(buf);
    }

    QDataStream ds(buf);
    ds >> sharemode >> fragW >> fragH >> fragBufs >> x >> y;
    if(sharemode == DS_IMAGE_MODE)
    {

        if(fragBufs.size() == 0)
            return;
        if(image.width() != fragW*8 || image.height() != fragH*8)
        {
            image = QImage(fragW*8, fragH*8, QImage::Format_RGB888);
        }
        QPainter p(&image);
        for(auto it=fragBufs.begin(); it!=fragBufs.end(); ++it)
        {
            int k = it.key();
            QByteArray buf = it.value();
            QImage v;
            v.loadFromData(buf, "JPEG");

            int col = k/8;
            int row = k%8;

            p.drawImage(col*fragW, row*fragH, v);

        }
        p.end();
    }
    else
    {
        if(fragBufs.size() == 0)
            return;
        if(image.width() != fragW*8 || image.height() != fragH*8)
        {
            image = QImage(fragW*8, fragH*8, QImage::Format_RGB888);
        }
        QPainter p(&image);
        QImage::Format format = sharemode == DS_TEXT_MODE ?
                    QImage::Format_RGB444:QImage::Format_RGB888;
        for(auto it=fragBufs.begin(); it!=fragBufs.end(); ++it)
        {
            int k = it.key();
            QByteArray buf = it.value();
            QImage v((uchar*)buf.data(), fragW, fragH, format);

            int col = k/8;
            int row = k%8;

            p.drawImage(col*fragW, row*fragH, v);

        }
        p.end();
    }

    emit this->sigImage(image, x, y);
    packets.remove(frameIdx);
}

void DSRecv::slotReadyRead()
{
    while(sock->hasPendingDatagrams())
    {
        qint64 len = sock->pendingDatagramSize();
        QByteArray buf(len, 0);
        sock->readDatagram(buf.data(), buf.size(), &this->_addr);

        QDataStream ds(buf);
        quint32 frameIdx;
        quint16 index;
        quint16 total;
        QByteArray sub;
        ds >> index >> total >> sub >> frameIdx;

        packets[frameIdx][index] = sub;
        if(packets[frameIdx].size() == total)
        {
            combine(frameIdx);
        }
    }
}

void DSRecv::slotAsk(QByteArray buf)
{
    QJsonObject obj;
    obj.insert(DS_CMD, DS_QUESTION);
    obj.insert(DS_QUESTION, QString(buf.simplified()));
    buf = QJsonDocument(obj).toJson();
    this->sock->writeDatagram(buf, this->_addr, DS_PORT_DS);
}


