#ifndef DSVIEW_H
#define DSVIEW_H

#include <QWidget>
#include <QImage>
#include <QMenu>
#include <QTextEdit>
#include "DSRecv.h"
#include <QPushButton>
class DSView : public QWidget
{
    Q_OBJECT
public:
    explicit DSView(QWidget *parent = 0);

    DSRecv* _recv;
    QImage _image;
    QMenu* _menu;

    QAction* _actTop;
#define SHOW_FIT_WINDOW 1
#define SHOW_NOBORDER 0
#define SHOW_ALL 2
    uint32_t _showmode;

    QAction* _showmodeFit;
    QAction* _showmodeNoborder;
    QAction* _shownodeAll;

    bool _topMost;
    int _x, _y;
    void createMenu();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

    QWidget* _askWidget;
    QTextEdit* _askEdit;

signals:
    void sigAsk(QByteArray buf);

public slots:
    void slotImage(QImage image, int x, int y);
    void slotAsk();
};

#endif // DSVIEW_H
