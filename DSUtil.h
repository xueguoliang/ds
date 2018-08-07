#ifndef DSUTIL_H
#define DSUTIL_H

#include <QByteArray>
#include <QDebug>
#include <QJsonObject>
#include <QtEndian>
class DSUtil
{
public:
    DSUtil();

    static uchar* attachShm();
    static void detachShm(uchar*data );

    static void saveConfig(QJsonObject obj);
    static QJsonObject getConfig();

    static QString getVersion();
    static QString getDate();
    static void dumpVersion();

    static void installDebug();
};

#endif // DSUTIL_H
