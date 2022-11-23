#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QObject>
#include <QWidget>
#include <QToolButton>

#include "global_defs.h"
#include "Models.h"

class ToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ToolButton(const Unit &unit, OpType _op = OpType::CREATE, QWidget *parent = nullptr);
    explicit ToolButton(const int _row, OpType _op = OpType::DELETE_ROW, QWidget *parent = nullptr);

signals:
    void deleteUnit(Unit _unit);
    void changeUnit(Unit _unit);
    void deleteRow(int rowId);


private:
    Unit mUnit;
    OpType mOperation;
    int mRowId;


    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // TOOLBUTTON_H
