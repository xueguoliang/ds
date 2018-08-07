#include "DSDBConnector.h"
#include <QSqlDatabase>
#include "../DSDef.h"
#include "../DSUtil.h"
DSDBConnector::DSDBConnector(QJsonObject& config):_config(config)
{
     connectDB();
}

DSDBConnector::~DSDBConnector()
{
    disconnectDB();
}

bool DSDBConnector::connectDB()
{
    if(_config.isEmpty())
        _config = DSUtil::getConfig();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(_config.value(DS_HOSTNAME).toString());
    db.setUserName(_config.value(DS_USERNAME).toString());
    db.setPassword(_config.value(DS_PASSWORD).toString());
    db.setDatabaseName(_config.value(DS_DATABASE).toString());

    return db.open();
}

void DSDBConnector::disconnectDB()
{
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}


