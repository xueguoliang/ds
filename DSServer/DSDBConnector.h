#ifndef DSDBCONNECTOR_H
#define DSDBCONNECTOR_H

#include <QJsonObject>
class DSDBConnector
{
public:
    DSDBConnector(QJsonObject& config);
    ~DSDBConnector();

    QJsonObject& _config;

    bool connectDB();
    void disconnectDB();
};

#endif // DSDBCONNECTOR_H
