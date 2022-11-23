#include "TableWidget.h"

TableWidget::TableWidget(const QList<Unit> &_units, QWidget *parent) :
    QTableWidget(parent),
    mUnitList(_units)
{

}


