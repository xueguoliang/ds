#include "DSLogin.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include "DSDef.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include "DSRegister.h"
#include <QApplication>
#include "DSUtil.h"

DSLogin::DSLogin(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(windowFlags() &~Qt::WindowMinMaxButtonsHint);

    QGridLayout* grid = new QGridLayout(this);
    QHBoxLayout* hBox;

    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(4, 1);
    grid->setRowStretch(0, 1);
    grid->setRowStretch(4, 1);

    QLabel* label1;
    QLabel* label2;
    grid->addWidget(label1 = new QLabel("用户名:"), 1, 1);
    grid->addWidget(label2 = new QLabel("密  码:"), 2, 1);
    grid->addWidget(_username = new QLineEdit, 1, 2);
    grid->addWidget(_password = new QLineEdit, 2, 2);
    grid->addLayout(hBox = new QHBoxLayout, 3, 1, 1, 2);

    _password->setEchoMode(QLineEdit::Password);
    label1->setAlignment(Qt::AlignJustify);
    label2->setAlignment(Qt::AlignJustify);

    hBox->addStretch(1);
    hBox->addWidget(_button = new QPushButton("登陆"));
    QPushButton* reg;
    hBox->addWidget(reg = new QPushButton("注册"));
    connect(reg, SIGNAL(clicked(bool)), this, SLOT(slotRegister()));
    connect(_button, SIGNAL(clicked()), this, SLOT(slotLogin()));
    _button->setDefault(true);

    connect(&_man, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotLoginAck(QNetworkReply*)));

    _bReg = false;
}

void DSLogin::timerEvent(QTimerEvent *)
{
    QMessageBox::warning(this, "错误", "超时了");
    reject();
}

void DSLogin::slotRegister()
{
    _bReg = true;
    reject();
//    QString url=QString("http://www.ds-project.cn:%1/register.html").arg(DS_PORT_HTTP);
//    QDesktopServices::openUrl(QUrl(url));
}

void DSLogin::slotLogin()
{
    QString username = _username->text();
    QString password = _password->text();
    if(username.length() == 0 || password.length() == 0)
    {
        return;
    }

    _button->setEnabled(false);
  //  startTimer(5000);

    QJsonObject obj;
    obj.insert(DS_USERNAME, username);
    obj.insert(DS_PASSWORD, password);
    obj.insert(DS_APP, QApplication::instance()->applicationName());
    obj.insert(DS_CMD, QString(DS_LOGIN));
    obj.insert(DS_DATE, DSUtil::getDate());
    obj.insert(DS_VERSION, DSUtil::getVersion());

    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_0);

    QString url=QString("https://www.ds-project.cn:%1/user").arg(DS_PORT_HTTPS);
    QNetworkRequest req;
    req.setSslConfiguration(config);
    req.setUrl(QUrl(url));
    _man.post(req, QJsonDocument(obj).toJson());
}

void DSLogin::slotLoginAck(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::warning(this, "错误", "网络错误");
        reject();
        return;
    }

    QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
    if(obj.value(DS_RESULT) == DS_OK)
    {
        _session = obj.value(DS_SESSION).toString();
        _adUrl = obj.value(DS_ADURL).toString();
        accept();
    }
    else
    {
        QMessageBox::warning(this, "登陆错误", obj.value(DS_REASON).toString());
        reject();
    }
}
