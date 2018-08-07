
#include <QApplication>
#include "DSApp.h"
#include "../DSUtil.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    DSUtil::installDebug();

    qWarning() << "main:" << QThread::currentThreadId();

    new DSApp;
    return app.exec();
}


