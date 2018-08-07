#ifndef DSSEND_H
#define DSSEND_H

#include <QThread>
#include <QUdpSocket>

class DSSend : public QObject
{
    Q_OBJECT
public:
    explicit DSSend(QObject *parent = 0);

    QUdpSocket* sock;
    QThread* thread;
private:
    QString shareaddr;


signals:

public slots:
    void slotBuffer(QByteArray buf, quint32 frameIdx);
    void slotShareAddrChanged(QString addr)
    {
        shareaddr = addr;
    }
};

#endif // DSSEND_H
