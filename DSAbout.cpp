#include "DSAbout.h"
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QPushButton>
#include "DSUtil.h"
#include <QCoreApplication>
#include <QGridLayout>
#include <QLabel>
#include <QDesktopServices>

void DSAbout::about()
{
    QDesktopServices::openUrl(QUrl("http://www.xueguoliang.cn/ds/about.html"));
}
