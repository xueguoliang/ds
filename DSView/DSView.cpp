#include "DSView.h"
#include <QPainter>
#include "../DSAbout.h"
#include <QMouseEvent>
#include <QCursor>
#include <QTimer>
#include <QHBoxLayout>
#include <QDesktopServices>
#include <QMessageBox>
DSView::DSView(QWidget *parent) : QWidget(parent)
{
    createMenu();
    this->_recv = new DSRecv;
    connect(_recv, SIGNAL(sigImage(QImage, int, int)), this, SLOT(slotImage(QImage, int, int)));
    connect(this, SIGNAL(sigAsk(QByteArray)), _recv, SLOT(slotAsk(QByteArray)));
}

void DSView::createMenu()
{
    _menu = new QMenu;

    _topMost = false;
    _showmodeFit = _menu->addAction("按窗口比例缩放", [&](){
        _showmodeFit->setChecked(false);
        _showmodeNoborder->setChecked(false);
        _shownodeAll->setChecked(false);
        this->_showmodeFit->setChecked(true);

        this->_showmode = SHOW_FIT_WINDOW;

        update();
    });
    _showmodeNoborder = _menu->addAction("不缩放", [&](){
        _showmodeFit->setChecked(false);
        _showmodeNoborder->setChecked(false);
        _shownodeAll->setChecked(false);
        this->_showmodeNoborder->setChecked(true);
        this->_showmode = SHOW_NOBORDER;
        update();
    });
    _shownodeAll = _menu->addAction("按讲师图像比例缩放", [&](){
        _showmodeFit->setChecked(false);
        _showmodeNoborder->setChecked(false);
        _shownodeAll->setChecked(false);
        this->_shownodeAll->setChecked(true);
        this->_showmode = SHOW_ALL;
        update();
    });

    _menu->addAction("提问", [&](){
        QWidget* w = new QWidget;
        w->setWindowTitle("课堂提问");
        QVBoxLayout* l = new QVBoxLayout(w);
        QTextEdit* e;
        QHBoxLayout* h;
        l->addWidget(e = new QTextEdit);
        l->addLayout(h = new QHBoxLayout);
        h->addStretch(1);
        QPushButton* button;
        h->addWidget(button = new QPushButton("发送"));
        connect(button, SIGNAL(clicked()), SLOT(slotAsk()));
        h->addWidget(button = new QPushButton("关闭"));
        connect(button, SIGNAL(clicked()), w, SLOT(deleteLater()));
        w->show();
        _askWidget = w;
        _askEdit = e;
    });

    _showmodeFit->setCheckable(true);
    _showmodeNoborder->setCheckable(true);
    _shownodeAll->setCheckable(true);

    this->_showmode = SHOW_NOBORDER;

    _showmodeFit->setChecked(false);
    _showmodeNoborder->setChecked(false);
    _shownodeAll->setChecked(false);
    _showmodeNoborder->setChecked(true);

#ifdef Q_OS_WIN
    _actTop = _menu->addAction("窗口置顶", [&](){
        this->_topMost = !this->_topMost;
        _actTop->setChecked(this->_topMost);

        if(_topMost)
        {
            this->setWindowFlags(Qt::WindowStaysOnTopHint|windowFlags());
        }
        else
        {
            this->setWindowFlags(Qt::WindowType((~Qt::WindowStaysOnTopHint)& windowFlags()));
        }
        this->show();
    });
    _actTop->setCheckable(true);
#endif
    _menu->addAction("关于", [](){
        DSAbout::about();
    });
}

void DSView::paintEvent(QPaintEvent *)
{

    QPainter p(this);
    if(_image.isNull())
        return;

    QImage image;

    if(this->_showmode == SHOW_FIT_WINDOW)
    {
        image = _image.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        p.drawImage(0, 0, image);
    }
    else if(this->_showmode == SHOW_ALL)
    {
        if(height() == 0)
            return;
        if(_image.height() == 0)
            return;

         //               9    /   6 = 1.5
        double rImage = _image.width() * 1.0 / _image.height();
           //           2     /    1 =  2
        double rWin = width() * 1.0 / height();

        if(rWin > rImage)
            image = _image.scaledToHeight(height(), Qt::SmoothTransformation);
        else
            image = _image.scaledToWidth(width(), Qt::SmoothTransformation);
        p.drawImage(0, 0, image);
    }
    else
    {
        int xTrans = 0;
        int yTrans = 0;
        if(width() < _image.width())
        {
            if(_x > width())
                xTrans = width() - _image.width();
        }
        else
        {
            xTrans = (width() - _image.width())/2;
        }
        if(height() < _image.height())
        {
            if(_y > height())
                yTrans = height() - _image.height();
        }
        else
        {
            yTrans = (height() - _image.height())/2;
        }
        p.translate(xTrans, yTrans);
        p.drawImage(0, 0, _image);
    }
}

void DSView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        _menu->popup(QCursor::pos());
    }
}

void DSView::slotImage(QImage image, int x, int y)
{
    _x = x;
    _y = y;
    _image = image;
    update();
}

void DSView::slotAsk()
{
    QString str = this->_askEdit->document()->toPlainText();
    if(str.size() == 0)
        return;

    QByteArray buf = str.toUtf8();

    emit this->sigAsk(buf);
    _askWidget->deleteLater();
}
