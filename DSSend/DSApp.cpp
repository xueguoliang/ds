#include "DSApp.h"
#include "../DSDef.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "../DSUtil.h"
#include <QImage>
#include "DSSend.h"
#include "DSFrame.h"
#include <QCoreApplication>
DSApp::DSApp(QObject *parent) : QObject(parent)
{
    if(!_sock.bind(QHostAddress::LocalHost, DS_PORT_SEND))
    {
        QCoreApplication::instance()->quit();
        return;
    }
    connect(&_sock, &QUdpSocket::readyRead, [&](){
        readDatagram();
    });

    _shm = NULL;
    attachMemory();

    send = new DSSend;
    frame = new DSFrame;

    connect(this, &DSApp::sigNewFrame,
            frame, &DSFrame::slotNewFrame);
    connect(frame, &DSFrame::sigBuffer,
            send, &DSSend::slotBuffer);

    connect(this, SIGNAL(sigShareAddrChanged(QString)),
            send, SLOT(slotShareAddrChanged(QString)));
    connect(this, SIGNAL(sigShareModeChanged(QString)),
            frame, SLOT(slotShareModeChange(QString)));
}

DSApp::~DSApp()
{
    send->thread->quit();
    frame->thread->quit();
    detachMemory();
}

void DSApp::readDatagram()
{
    while(_sock.hasPendingDatagrams())
    {
        quint64 len = _sock.pendingDatagramSize();
        QByteArray buf(len, 0);
        _sock.readDatagram(buf.data(), buf.size());

        QJsonDocument doc = QJsonDocument::fromJson(buf);
        QJsonObject obj = doc.object();

        if(obj.value(DS_CMD).toString() == DS_CAPTURE)
        {
            int x = *(uint32_t*)_shm;
            int y = *(uint32_t*)(_shm+4);
            QImage image((uchar*)_shm+8, _width, _height, QImage::Format_RGB888);
            emit sigNewFrame(image, x, y);
        }
        else if(obj.value(DS_CMD).toString() == DS_SETUP)
        {
            if(obj.find(DS_SHARE_ADDR) != obj.end())
            {
              //  send->shareaddr = obj.value(DS_SHARE_ADDR).toString();
                emit this->sigShareAddrChanged(obj.value(DS_SHARE_ADDR).toString());
            }
            if(obj.find(DS_WIDTH) != obj.end())
            {
                _width = obj.value(DS_WIDTH).toInt();
            }
            if(obj.find(DS_HEIGHT) != obj.end())
            {
                _height = obj.value(DS_HEIGHT).toInt();
            }
            if(obj.find(DS_SHARE_MODE) != obj.end())
            {
               // frame->_sharemode = obj.value(DS_SHARE_MODE).toString();
              //  qWarning() << "share mode is: " << frame->_sharemode;
                emit this->sigShareModeChanged(obj.value(DS_SHARE_MODE).toString());
            }
        }
    }
}

void DSApp::attachMemory()
{
    if(_shm == NULL)
        _shm = DSUtil::attachShm();
}

void DSApp::detachMemory()
{
    if(this->_shm)
    {
        DSUtil::detachShm(this->_shm);
    }
}

