#ifndef DSLOGIN_H
#define DSLOGIN_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class DSLogin : public QDialog
{
    Q_OBJECT
public:
    explicit DSLogin(QWidget *parent = 0);

    QLineEdit* _username;
    QLineEdit* _password;
    QPushButton* _button;
    void timerEvent(QTimerEvent *event);
    QNetworkAccessManager _man;

    QString _session;
    QString _adUrl;
    bool _bReg;
signals:

public slots:
    void slotRegister();
    void slotLogin();
    void slotLoginAck(QNetworkReply* reply);
};

#endif // DSLOGIN_H
