#ifndef DSSETUP_H
#define DSSETUP_H

#include <QDialog>
#include <QComboBox>
#include <QJsonObject>

class DSSetup : public QDialog
{
    Q_OBJECT
public:
    explicit DSSetup(QWidget *parent = 0);

    enum{ITEM_SHARE_ADDR, ITEM_SHARE_MODE, ITEM_MAX};

    QComboBox* items[ITEM_MAX];
    QVector<QString> keys;

    void init(QJsonObject obj);
    QJsonObject get();
signals:

public slots:
};

#endif // DSSETUP_H
