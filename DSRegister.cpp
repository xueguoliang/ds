#include "DSRegister.h"
#include <QMessageBox>
DSRegister::DSRegister(QWidget *parent) : QDialog(parent)
{
    QGridLayout* grid = new QGridLayout(this);

    int row = 1;

    grid->setRowStretch(0, 1);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(3, 1);

    grid->addWidget(new QLabel("用户名："), row++, 1);
    grid->addWidget(new QLabel("密码："), row++, 1);
    grid->addWidget(new QLabel("重复密码："), row++, 1);
    grid->addWidget(new QLabel("QQ："), row++, 1);
    grid->addWidget(new QLabel("手机："), row++, 1);

    /*
    QLineEdit* _username;
    QLineEdit* _password;
    QLineEdit* _passwordVerify;
    QLineEdit* _qq;
    QLineEdit* _mobile;
*/
    row = 1;
    grid->addWidget(_username = new QLineEdit, row++, 2);
    grid->addWidget(_password = new QLineEdit, row++, 2);
    grid->addWidget(_passwordVerify = new QLineEdit, row++, 2);
    grid->addWidget(_qq = new QLineEdit, row++, 2);
    grid->addWidget(_mobile = new QLineEdit, row++, 2);

    _password->setEchoMode(QLineEdit::Password);
    _passwordVerify->setEchoMode(QLineEdit::Password);

    QHBoxLayout* hBox;
    grid->addLayout(hBox=new QHBoxLayout, row++, 1, 1, 2);
    hBox->addStretch(1);
    hBox->addWidget(_button = new QPushButton("注册"));

    grid->setRowStretch(row++, 1);

    connect(&_man, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReisterAck(QNetworkReply*)));
    connect(_button, SIGNAL(clicked()), this, SLOT(slotRegister()));
}

void DSRegister::slotRegister()
{
    if(_username->text().length() == 0)
        return ;

    if(_password->text().length() == 0)
        return;

    if(_password->text() != _passwordVerify->text())
    {
        QMessageBox::warning(this, "错误", "密码不一致");
        return;
    }

    if(_qq->text().length() == 0)
        return;
    if(_mobile->text().length() == 0)
        return;

    QJsonObject obj;
    obj.insert(DS_CMD, QString(DS_REGISTER));
    obj.insert(DS_USERNAME, _username->text());
    obj.insert(DS_PASSWORD, _password->text());
    obj.insert(DS_QQ, _qq->text());
    obj.insert(DS_MOBILE, _mobile->text());

    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_0);

    QByteArray buf = QJsonDocument(obj).toJson();

    QString url=QString("https://www.ds-project.cn:%1/user").arg(DS_PORT_HTTPS);
    QNetworkRequest req;
    req.setSslConfiguration(config);
    req.setUrl(QUrl(url));
    _man.post(req, buf);
}

void DSRegister::slotReisterAck(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::warning(this, "错误", reply->errorString());
        return;
    }

    QByteArray buf = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(buf);
    QJsonObject obj = doc.object();

    if(obj.value(DS_RESULT).toString() == QString(DS_OK))
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this, "错误", obj.value(DS_REASON).toString());
    }
}
