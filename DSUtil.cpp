#include "DSUtil.h"
#include "DSDef.h"
#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#endif
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <QDir>

DSUtil::DSUtil()
{

}

#ifdef Q_OS_WIN
static HANDLE g_fileMapping;
uchar* DSUtil::attachShm()
{

    g_fileMapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, DS_SHARE_FILE_NAME);
    if(!g_fileMapping)
    {
        g_fileMapping = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
                                           0, DS_MAP_LENGTH, DS_SHARE_FILE_NAME);
    }
    return (uchar*)MapViewOfFile(g_fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
}

void DSUtil::detachShm(uchar *data)
{
    UnmapViewOfFile((LPCVOID)data);
    CloseHandle(g_fileMapping);
}
#else
uchar *DSUtil::attachShm()
{
    uchar* shm;
    int fd = shm_open(DS_SHARE_FILE_NAME, O_RDWR|O_CREAT|O_EXCL, 0777);
    if(fd > 0)
    {
        ftruncate(fd, DS_MAP_LENGTH);
    }
    else
    {
        fd = shm_open(DS_SHARE_FILE_NAME, O_RDWR, 0777);
    }

    shm = (uint8_t*)mmap(NULL, DS_MAP_LENGTH, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(shm == NULL)
    {
        qWarning() << "map share memory error";
        return NULL;
    }
    close(fd);

    return shm;

}

void DSUtil::detachShm(uchar *data)
{
    munmap(data, DS_MAP_LENGTH);
}
#endif



void DSUtil::saveConfig(QJsonObject obj)
{
    QJsonDocument doc(obj);
    QByteArray buf = doc.toJson();
    QFile file(QDir::homePath() + "/dsconfig");
    file.open(QFile::WriteOnly);
    file.write(buf);
    file.close();
}

QJsonObject DSUtil::getConfig()
{
    QFile file(QDir::homePath() + "/dsconfig");
    file.open(QFile::ReadOnly);
    QByteArray buf = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(buf);
    return doc.object();
}

QString DSUtil::getVersion()
{
    QFile file(":/version.txt");
    file.open(QFile::ReadOnly);
    QByteArray buf = file.readAll();
    file.close();
    return buf.simplified();
}

QString DSUtil::getDate()
{
    QFile file(":/date.txt");
    file.open(QFile::ReadOnly);
    QByteArray buf = file.readAll();
    file.close();
    return buf.simplified();
}

void DSUtil::dumpVersion()
{
    qWarning() << QString("%1, %2").arg(getVersion() ,getDate());
}

static void MsgHandler(QtMsgType type, const QMessageLogContext & , const QString &msg)
{
    if(type == QtDebugMsg)return;
    QString cat = "default";
    if(type == QtWarningMsg)
    {
        cat = "warn";
    }
    else if(type == QtCriticalMsg)
    {
        cat = "error";
    }

    QString filename = QDir::homePath() + "/dslog/" + qApp->applicationName() + ".txt";
    QFile file(filename);
    file.open(QIODevice::Append | QIODevice::WriteOnly);
#ifdef Q_OS_WIN
    QString str = QString("%1:%2\r\n").arg(cat, msg);
#else
    QString str = QString("%1:%2\n").arg(cat, msg);
#endif
    file.write(str.toUtf8());
    file.close();
}

void DSUtil::installDebug()
{
    QDir().mkdir(QDir::homePath() + "/dslog");
#ifdef QT_NO_DEBUG
    qInstallMessageHandler(MsgHandler);
#endif

    dumpVersion();
}

