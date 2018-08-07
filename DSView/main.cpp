
#include <QApplication>
#include "DSRecv.h"
#include "DSView.h"
#include "../DSLogin.h"
#include "../DSRegister.h"
#include "../DSUtil.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    DSUtil::installDebug();

    DSView* p = new DSView;

    p->show();

    return app.exec();
}
