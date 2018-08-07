#include "DSCap.h"
#include <QCoreApplication>
#include "../DSUtil.h"
#include <QCursor>
DSCap::DSCap(QObject *parent) : QObject(parent)
{
    this->moveToThread(&_thread);
    connect(&_thread, SIGNAL(finished()), this, SLOT(slotThreadStopped()));
    this->_iFmtCtx = NULL;
    this->_iFmt = NULL;
    _thread.start();
    _checked = false;
    _timer = NULL;
}

void DSCap::start(int interval, int scale, int width, int height)
{
    this->_interval = interval;
    //this->_quit = false;
    this->_scale = scale;
    this->_width = width;
    this->_height = height;
    init();

    this->_timer = new QTimer;
    _timer->setInterval(this->_interval);
    connect(_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
    _timer->start();
}

void DSCap::stop()
{
    _timer->stop();
    _timer->deleteLater();
    _thread.quit();
    _timer = NULL;
}

void DSCap::openInput()
{
#ifdef Q_OS_WIN
    const char* device = "gdigrab";
    const char* url = "desktop";
#endif

#ifdef Q_OS_MAC
    const char* device = "avfoundation";
    const char* url = "1";
#endif

#ifdef Q_OS_LINUX
    const char* device = "x11grab";
    const char* url = ":0.0";
#endif

    this->_iFmt = av_find_input_format(device);
    AVDictionary *options = NULL;
    av_dict_set(&options, "capture_cursor", "1", 0);
    //av_dict_set(&options, "capture_mouse_clicks", "1", 0);
    //  av_dict_set(&options, "pix_fmt", "rgb24", 0);
    av_dict_set(&options, "framerate", QString::number(1000/_interval).toUtf8().data(), 0);

    QString screenSize = QString("%1x%2").arg(_width).arg(_height);
    qWarning() << "screen size is" << screenSize;
    av_dict_set(&options, "video_size", screenSize.toUtf8().data(), 0);
    //  av_dict_set(&options, "vcodec", "rawvideo", 0);
    int ret = avformat_open_input(&this->_iFmtCtx, url, this->_iFmt, &options);
    if(ret != 0)
    {
        qWarning() << "open input device error" << "device:" << device << "url:" << url;
        qApp->quit();
        return;
    }
    avformat_find_stream_info(_iFmtCtx, &options);

}

void DSCap::initMedia()
{
    for(_idx = 0; _idx < _iFmtCtx->nb_streams; _idx++)
    {
        if(this->_iFmtCtx->streams[_idx]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
            break;
    }

    _inputCodecCtx = _iFmtCtx->streams[_idx]->codec;

    qWarning() << "video idx" << _idx;

    // alloc decoder
    AVCodec* decoder = avcodec_find_decoder(_inputCodecCtx->codec_id);
    if(decoder == NULL)
    {
        qWarning() << "Could not find decoder";
        return;
    }
    else
    {
        qWarning() << "decoder is" << decoder->name;
    }

    // open decoder
    int ret = avcodec_open2(_inputCodecCtx, decoder, NULL);
    if(ret < 0)
    {
        qWarning() << "open decoder failure";
        return;
    }

    qWarning() << "_inputCodecCtx->width" << _inputCodecCtx->width
               << "_inputCodecCtx->height" << _inputCodecCtx->height;

    _dstWidth = _inputCodecCtx->width*_scale/100;
    _dstHeight = _inputCodecCtx->height*_scale/100;

    qWarning() << "_dstWidth" << _dstWidth << "_dstHeight" << _dstHeight;

    sendSizeToDSSend();

    // 共享屏幕使用的转码
    _cvtCtxShare = sws_getContext(_inputCodecCtx->width, _inputCodecCtx->height,
                                  _inputCodecCtx->pix_fmt, _dstWidth, _dstHeight,
                                  AV_PIX_FMT_RGB24, SWS_BICUBLIN, NULL, NULL, NULL);

    _inputPacket = (AVPacket*)av_malloc(sizeof(*_inputPacket));
    _inputFrame = av_frame_alloc();
    _outputRGBFrame = av_frame_alloc();
    avpicture_fill((AVPicture*)_outputRGBFrame, (uint8_t*)_shm+8,
                   AV_PIX_FMT_RGB24, _dstWidth, _dstHeight);
}

void DSCap::init()
{
    _shm = DSUtil::attachShm();
    openInput();
    initMedia();
}

void DSCap::loop()
{
    int ret = av_read_frame(_iFmtCtx, _inputPacket);
    if(_inputPacket->stream_index != _idx)
    {
        av_free_packet(_inputPacket);
        return;
    }
    if(ret >= 0 && _checked)
    {
        int got_picture = 0;
        ret = avcodec_decode_video2(_inputCodecCtx,
                                    _inputFrame,
                                    &got_picture,
                                    _inputPacket);
        // 缩放，缩放的内容放入共享内存
        sws_scale(_cvtCtxShare,
                  _inputFrame->data,
                  _inputFrame->linesize,
                  0,
                  _inputCodecCtx->height,
                  _outputRGBFrame->data,
                  _outputRGBFrame->linesize);

        QPoint pt = QCursor::pos();
        *(uint32_t*)(_shm) = pt.x();
        *(uint32_t*)(_shm+4) = pt.y();

        if(_timer)
            sendCaptureOne();

    }
    av_free_packet(_inputPacket);
}

void DSCap::fini()
{
    sws_freeContext(_cvtCtxShare);
    av_frame_free(&_inputFrame);
    av_frame_free(&_outputRGBFrame);
    avcodec_close(_inputCodecCtx);
    avformat_close_input(&_iFmtCtx);

    DSUtil::detachShm(_shm);
}

void DSCap::sendSizeToDSSend()
{
    QJsonObject obj;
    obj.insert(DS_CMD, DS_SETUP);
    obj.insert(DS_WIDTH, this->_dstWidth);
    obj.insert(DS_HEIGHT, this->_dstHeight);
    QJsonDocument doc(obj);
    QByteArray buf = doc.toJson();

    _sock->writeDatagram(buf, QHostAddress::LocalHost, DS_PORT_SEND);
}

void DSCap::sendCaptureOne()
{
    QJsonObject obj;
    obj.insert(DS_CMD, DS_CAPTURE);
    QJsonDocument doc(obj);
    QByteArray buf = doc.toJson();

    this->_sock->writeDatagram(buf, QHostAddress::LocalHost, DS_PORT_SEND);
}

void DSCap::slotTimeout()
{
    this->loop();
}

void DSCap::slotThreadStopped()
{
    qWarning() << "DSCap stopped...";
    this->fini();
    this->deleteLater();
}
