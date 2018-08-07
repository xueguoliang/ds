#include "DSApp.h"
#include "DSSetup.h"
#include "../DSDef.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>

#include <QDesktopWidget>
#include <QScreen>
#include <QThread>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QPainter>
#include "../DSLogin.h"
#include "../DSUtil.h"
#include "../DSAbout.h"
#include "../DSRegister.h"
#include <QMessageBox>

DSApp::DSApp(QObject *parent) : QObject(parent)
{
    _started = false;
    initTray();
    initSocket();

    // keys<<DS_SHARE << DS_SHARE_ADDR << DS_SHARE_MODE << DS_PUSH << DS_PUSH_ADDR << DS_RECORD;
    static const char* keys[] = {
        DS_SHARE_ADDR,
        DS_SHARE_MODE
    };
    static const char* values[] =
    {
        "255.255.255.255",
        DS_HD_MODE,
    };
    this->_setup = DSUtil::getConfig();

    for(quint32 i=0; i<sizeof(keys)/sizeof(*keys); ++i)
    {
        if(_setup.value(keys[i]).isNull())
            _setup.insert(keys[i], QString(values[i]));
    }
}


DSApp::~DSApp()
{

}

void DSApp::initTray()
{
    _start = QPixmap(":/logo.png");
    _stop = QPixmap(":/logo.png");

    QSize sz = _start.size();

    QPainter p(&_start);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(Qt::red);
    p.setPen(Qt::NoPen);

    p.drawEllipse(QPoint(_start.width()/2, _start.height()/2), sz.width()/3, sz.width()/3);
    p.end();

    _contextMenu = new QMenu;

    _actionStart = _contextMenu->addAction("开始", [&](){
        start();

    });
    _actionStop = _contextMenu->addAction("停止", [&](){
        stop();

    });
    _contextMenu->addAction("设置", [&](){
        DSSetup setup;
        setup.init(_setup);
        setup.exec();
        _setup = setup.get();
        DSUtil::saveConfig(_setup);
        sendSetup();
    });

    _contextMenu->addAction("关于", [](){
        DSAbout::about();
    });

    _contextMenu->addAction("退出", [&](){
        if(_started)
            stop();
#ifdef Q_OS_WIN
        _cap.kill();
        _send.kill();
#endif

#ifdef Q_OS_LINUX
        _cap.kill();
        _send.kill();
#endif
        qApp->quit();
    });

    _actionStop->setDisabled(true);

    _tray = new QSystemTrayIcon;
    _tray->setContextMenu(this->_contextMenu);
    _tray->setIcon(QIcon(_stop));
    _tray->setToolTip(tr("桌面共享工具"));
    _tray->show();
}

void DSApp::initSocket()
{
    _sock = new QUdpSocket(this);
    _sock->bind(QHostAddress::Any, DS_PORT_DS);


    connect(_sock, &QUdpSocket::readyRead, [&](){
        recvData();
    });
}

void DSApp::start()
{
    if(_started)
        return;
    this->_logoTimer = startTimer(800);
    _started = true;
    _actionStart->setDisabled(true);
    _actionStop->setEnabled(true);
    _tray->setIcon(_start);

    sendSetup();

    QThread::msleep(100);

    {
        QJsonObject obj = _setup;
        obj.insert(DS_CMD, DS_START);
        obj.insert(DS_INTERVAL, 100);
        obj.insert(DS_SESSION, QString(this->_session));

        int scale = 100;
        if(isRetina())
            scale = 50;
        obj.insert(DS_SCALE, scale);

        qWarning() << "DS detect screen size is " << _screenSize;
        obj.insert(DS_WIDTH, _screenSize.width());
        obj.insert(DS_HEIGHT, _screenSize.height());

        QJsonDocument doc(obj);
        QByteArray buf = doc.toJson();

        this->_sock->writeDatagram(buf, QHostAddress::LocalHost, DS_PORT_CAP);
    }
}

void DSApp::stop()
{
    if(!_started)
        return;
    killTimer(this->_logoTimer);
    _started = false;
    _actionStart->setDisabled(false);
    _actionStop->setEnabled(false);
    _tray->setIcon(_stop);

    QJsonObject obj;
    obj.insert(DS_CMD, DS_STOP);

    QJsonDocument doc(obj);
    QByteArray buf = doc.toJson();

    this->_sock->writeDatagram(buf, QHostAddress::LocalHost, DS_PORT_CAP);

    QThread::usleep(100);
}

void DSApp::timerEvent(QTimerEvent *ev)
{
    if(ev->timerId() == this->_logoTimer)
    {
        static bool log = true;
        log = !log;
        this->_tray->setIcon(log?this->_start:this->_stop);
    }
}

bool DSApp::isRetina()
{
    QRect rc;
    QScreen* screen;

    screen = QApplication::primaryScreen();
    rc = screen->geometry();

    QPixmap image = screen->grabWindow(-1);
    qDebug() << image.width() << rc.width();

    _screenSize = rc.size();

    if(image.width() == rc.width()*2)
    {
        qWarning() << "isRetina, image size is:" << image.size() <<" screen size:" << rc.size();
        return true;
    }



    qWarning() << "not Retina, image size is:" << image.size() <<" screen size:" << rc.size();
    return false;
}

void DSApp::sendSetup()
{
    QJsonObject obj = _setup;
    obj.insert(DS_CMD, DS_SETUP);
    obj.insert(DS_SESSION, QString(this->_session));
    QJsonDocument doc(obj);
    QByteArray buf = doc.toJson();
    this->_sock->writeDatagram(buf, QHostAddress::LocalHost, DS_PORT_SEND);
}

void DSApp::recvData()
{
    while(_sock->hasPendingDatagrams())
    {
        QHostAddress addr;
        quint16 port;
        int len = _sock->pendingDatagramSize();
        QByteArray buf(len, 0);
        _sock->readDatagram(buf.data(), buf.size(), &addr, &port);
        QJsonObject obj = QJsonDocument::fromJson(buf).object();
        if(obj.value(DS_CMD).toString() == DS_STOP)
        {
            this->stop();
            QMessageBox::critical(NULL, "错误", obj.value(DS_REASON).toString());
        }
        if(obj.value(DS_CMD).toString() == DS_QUESTION)
        {
            QFile file(QDir::homePath() + "/ds_question.txt");
            file.open(QFile::WriteOnly|QFile::Append);
#ifdef Q_OS_WIN
            QString str = obj.value(DS_QUESTION).toString() + "\r\n";
#else
            QString str = obj.value(DS_QUESTION).toString() + "\n";
#endif
            file.write(str.toUtf8());
            file.close();
            // 不要响应这个消息，这个socket是收图片的socket，如果响应反而出问题了。
        }
    }
}


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    DSUtil::installDebug();

    DSApp* dsapp = NULL;
    app.setQuitOnLastWindowClosed(false);
#if 1
    {
        DSLogin login;

        while(1)
        {
            login._bReg = false;
            if(login.exec() != QDialog::Accepted)
            {
                if(login._bReg)
                {
                    DSRegister reg;
                    reg.exec();
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                break;
            }
        }

        dsapp = new DSApp;
        dsapp->_session = login._session;
        dsapp->_adurl = login._adUrl;
    }
#endif

#ifdef Q_OS_LINUX
    if(dsapp)
    {
        QDir dir(argv[0]);
        dir.cd("..");
        QString path =  dir.absolutePath();

        dsapp->_cap.start(path+"/DSCap.bin");
        dsapp->_send.start(path+"/DSSend.bin");
    }
#endif

#ifdef Q_OS_WIN
    if(dsapp)
    {
        QDir dir(argv[0]);
        dir.cd("..");
        QString path =  dir.absolutePath();

        dsapp->_cap.start(path+"/DSCap.exe");
        dsapp->_send.start(path+"/DSSend.exe");
    }
#endif

#ifdef Q_OS_OSX
    /* start services */
    {
        QProcess process;
        process.startDetached(
                    "launchctl",
                    QStringList()
                    << "unload"
                    << QDir::homePath()+
                    "/Library/LaunchAgents/cn.xueguoliang.dscap.plist" );
    }
    {
        QProcess process;
        process.startDetached(
                    "launchctl",
                    QStringList()
                    << "unload"
                    << QDir::homePath()+
                    "/Library/LaunchAgents/cn.xueguoliang.dssend.plist" );
    }
    QThread::msleep(100);
    {
        QProcess process;
        process.startDetached(
                    "launchctl",
                    QStringList()
                    << "load"
                    << QDir::homePath()+
                    "/Library/LaunchAgents/cn.xueguoliang.dscap.plist" );
    }
    {
        QProcess process;
        process.startDetached(
                    "launchctl",
                    QStringList()
                    << "load"
                    << QDir::homePath()+
                    "/Library/LaunchAgents/cn.xueguoliang.dssend.plist" );
    }
#endif

    return app.exec();
}
