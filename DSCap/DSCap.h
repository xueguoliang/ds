#ifndef DSCAP_H
#define DSCAP_H

#include <QThread>
#include <QUdpSocket>
#include "../DSDef.h"
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
}
#include <QTimer>
class DSCap : public QObject
{
    Q_OBJECT
public:
    explicit DSCap(QObject *parent = 0);
    QThread _thread;
    QUdpSocket* _sock;

    void start(int interval, int scale, int width, int height);
    void stop();

  //  bool event(QEvent *ev);

    void openInput();
    void initMedia();
    void init();
    void loop();
    void fini();

    int _dstWidth;
    int _dstHeight;

    int _interval;
    int _scale;
    // screen width and height
    int _width;
    int _height;

    QTimer* _timer;

    AVInputFormat* _iFmt;
    AVFormatContext* _iFmtCtx;

    uchar* _shm;

    AVCodecContext* _inputCodecCtx;
    int _idx;
    AVPacket* _inputPacket;
    AVFrame* _inputFrame;
    AVFrame* _outputRGBFrame;
    struct SwsContext* _cvtCtxShare;

    void sendSizeToDSSend();
    void sendCaptureOne();
    bool _checked;
signals:

public slots:
    void slotTimeout();
    void slotThreadStopped();
};

#endif // DSCAP_H
