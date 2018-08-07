#include "DSSetup.h"
#include <QGridLayout>
#include <QLabel>
#include <QNetworkAddressEntry>
#include "../DSDef.h"
DSSetup::DSSetup(QWidget *parent) : QDialog(parent)
{
    QGridLayout* grid = new QGridLayout(this);

    /*enum{ITEM_SHARE, ITEM_SHARE_ADDR, ITEM_SHARE_MODE,
         ITEM_PUSH, ITEM_PUSH_ADDR, ITEM_RECORD, ITEM_MAX};*/

    QMap<int, QStringList> models;
    models[ITEM_SHARE_ADDR] << "255.255.255.255";
    models[ITEM_SHARE_MODE] << DS_HD_MODE << DS_IMAGE_MODE<< DS_TEXT_MODE   ;
    QVector<bool> editable;
    editable <<  true << false ;

    keys << DS_SHARE_ADDR << DS_SHARE_MODE ;

    for(int row=0; row<ITEM_MAX; ++row)
    {
        grid->addWidget(new QLabel(keys[row]), row, 0);
        grid->addWidget(items[row]= new QComboBox(), row, 1);
        items[row]->addItems(models[row]);
        items[row]->setEditable(editable[row]);
        items[row]->setCurrentIndex(0);
    }

    QList<QNetworkAddressEntry> entrys;

    QList<QNetworkInterface> infs = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface inf, infs)
    {
        entrys.append(inf.addressEntries());
    }
    foreach(QNetworkAddressEntry entry, entrys)
    {
        if(entry.broadcast().toString().isEmpty())
            continue;
        qWarning() << "ip and broadcast ip:"
                   << entry.ip().toString()
                   << entry.broadcast().toString();
        items[ITEM_SHARE_ADDR]->addItem(entry.broadcast().toString());
    }

    items[ITEM_SHARE_ADDR]->setCurrentIndex(0);
}

void DSSetup::init(QJsonObject obj)
{
    for(int i=0; i<DSSetup::ITEM_MAX; ++i)
    {
        if(obj.value(keys[i]).toString().length() == 0) continue;
        this->items[i]->setCurrentText(obj.value(keys[i]).toString());
    }
}

QJsonObject DSSetup::get()
{
    QJsonObject obj;
    for(int i=0; i<DSSetup::ITEM_MAX; ++i)
    {
        obj.insert(keys[i], items[i]->currentText());
    }
    return obj;
}

