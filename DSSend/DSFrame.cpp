#include "DSFrame.h"
#include <QMap>
#include <QDebug>
#include "../DSDef.h"
#include <QBuffer>
DSFrame::DSFrame(QObject *parent) : QObject(parent)
{
    thread = new QThread;
    this->moveToThread(thread);
    thread->start();
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
}

QByteArray DSFrame::compress(QMap<int, QImage> frags, int fragW, int fragH, int x, int y)
{
    QByteArray buf;
    QMap<int, QByteArray> fragBufs;

    if(_sharemode == DS_IMAGE_MODE)
    {
        for(auto it = frags.begin(); it != frags.end(); ++it)
        {
            int key = it.key();
            QImage value = it.value();
            QBuffer fragBuf;
            fragBuf.open(QIODevice::ReadWrite);
            value.save(&fragBuf, "JPEG");
            fragBuf.close();
            fragBufs.insert(key, fragBuf.buffer());
        }

        QDataStream ds(&buf, QIODevice::ReadWrite);
        ds << _sharemode << fragW << fragH << fragBufs << x << y;

        buf += 'q';
    }
    else
    {
        for(auto it = frags.begin(); it != frags.end(); ++it)
        {
            int key = it.key();
            QImage value = it.value();
            QByteArray fragBuf((const char*)value.constBits(), value.byteCount());
            fragBufs.insert(key, fragBuf);
        }

        QDataStream ds(&buf, QIODevice::ReadWrite);
        ds << _sharemode << fragW << fragH << fragBufs << x << y;

        buf= qCompress(buf);
        buf += 'c';
    }
    return buf;
}

void DSFrame::slotNewFrame(QImage image, int x, int y)
{
    if(_sharemode == DS_TEXT_MODE)
    {
        image = image.convertToFormat(QImage::Format_RGB444);
    }
    else if(_sharemode == DS_HD_MODE)
    {
        image = image.convertToFormat(QImage::Format_RGB888);
    }

    static quint32 frameIdx = 0;
    if(frameIdx++ % 10 == 0)
        this->_frags.clear();

    QMap<int, QImage> frags;
    int fragW = image.width()/8;
    int fragH = image.height()/8;
    for(int col=0; col<8; ++col)
    {
        for(int row=0; row<8; ++row)
        {
            int key = col*8+row;
            QImage frag = image.copy(col*fragW, row*fragH, fragW, fragH);
            if(frag != _frags[key])
            {
                _frags[key] = frag;
                frags[key] = frag;
            }
        }
    }
    // default save as png
    //   QByteArray buf;
    //   QDataStream ds(&buf, QIODevice::ReadWrite);
    //   ds << frags;

    QByteArray buf = compress(frags, fragW, fragH, x, y);

    emit this->sigBuffer(buf, frameIdx);
}
