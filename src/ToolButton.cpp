#include "ToolButton.h"

ToolButton::ToolButton(const Unit &unit, OpType _op, QWidget *parent) :
    QToolButton(parent),
    mUnit(unit),
    mOperation(_op)
{
    setFixedSize(24, 24);

    switch (mOperation) {
    case OpType::DELETE:
        setIcon(QIcon(":/icons/51efe0e04f50db6a32d32eb749ebb30e.png"));
        break;
    case OpType::CHANGE:
        setIcon(QIcon(":/icons/6d1e481bdcf159961818823e652a7725.png"));
        break;
        // TODO Определить иконки для других вариантов
    default:
        break;
    }


}

ToolButton::ToolButton(const int _row, OpType _op, QWidget *parent) :
    QToolButton(parent),
    mOperation(_op),
    mRowId(_row)
{
    setFixedSize(24, 24);

    switch (mOperation) {
    case OpType::DELETE_ROW:
        setIcon(QIcon(":/icons/51efe0e04f50db6a32d32eb749ebb30e.png"));
        break;
        // TODO Определить иконки для других вариантов
    default:
        break;
    }
}

void ToolButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    if(mOperation == OpType::DELETE)
    {
        emit deleteUnit(mUnit);
    }
    else if(mOperation == OpType::CHANGE)
    {
        emit changeUnit(mUnit);
    }
    else if(mOperation == OpType::DELETE_ROW)
    {
        emit deleteRow(mRowId);
    }

}
