#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QObject>
#include <QTableWidget>

#include "Models.h"

class TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit TableWidget(const QList<Unit> &_units, QWidget *parent = nullptr);



signals:

private:
    QList<Unit> mUnitList;

};

#endif // TABLEWIDGET_H
