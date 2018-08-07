#ifndef DSFRAME_H
#define DSFRAME_H

#include <QThread>
#include <QImage>
#include <QMap>

class DSFrame : public QObject
{
    Q_OBJECT
public:
    explicit DSFrame(QObject *parent = 0);

    QThread* thread;

    QMap<int, QImage> _frags;



    QByteArray compress(QMap<int, QImage> frags, int fragW, int fragH, int x, int y);

private:
    QString _sharemode;

signals:
    void sigBuffer(QByteArray buf, quint32 frameIdx);

public slots:
    void slotNewFrame(QImage image, int x, int y);
    void slotShareModeChange(QString shareMode)
    {
        _sharemode = shareMode;
    }
};

#endif // DSFRAME_H
