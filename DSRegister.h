#ifndef DSREGISTER_H
#define DSREGISTER_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QUrl>
#include "DSDef.h"

class DSRegister : public QDialog
{
    Q_OBJECT
public:
    explicit DSRegister(QWidget *parent = 0);

    QLineEdit* _username;
    QLineEdit* _password;
    QLineEdit* _passwordVerify;
    QLineEdit* _qq;
    QLineEdit* _mobile;

    QPushButton* _button;

    QNetworkAccessManager _man;

signals:

public slots:
    void slotRegister();
    void slotReisterAck(QNetworkReply* reply);
};

#endif // DSREGISTER_H
