
#include <QCoreApplication>
#include "DSHttpServer.h"
#include "DSHttpsServer.h"
#include <QProcess>
#include <QFile>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    if(argc == 2)
    {
        QProcess process;
        process.startDetached(argv[0]);
        return 0;
    }

    QJsonObject config = DSUtil::getConfig();
    if(config.value(DS_ADURL).toString().length() == 0)
    {
        config.insert(DS_ADURL, QString("http://www.xueguoliang.cn"));
        DSUtil::saveConfig(config);
    }

    new DSHttpServer;
    new DSHttpsServer(config);
    return app.exec();

}
