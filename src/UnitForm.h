#ifndef UNITFORM_H
#define UNITFORM_H

#include <QWidget>
#include "global_defs.h"
#include "Models.h"
#include "NewPropertyForm.h"
#include "ToolButton.h"

namespace Ui {
class UnitForm;
}

class UnitForm : public QWidget
{
    Q_OBJECT

public:
    explicit UnitForm(QWidget *parent = nullptr);
    ~UnitForm();

signals:
    void createUnit(Unit _unit);
    void unitChanged(Unit unit);

public slots:
    void onOpenCreateForm();
    void onOpenUpdateForm(Unit unit);

private slots:


    void on_btnCreateProperty_clicked();

    void on_btnDeleteProperty_clicked();

    void on_btnSaveUnit_clicked();

    void on_btnCloseForm_clicked();

    void onCreateUnitProperty(UnitProperty _property);

    void onDeleteRow(int _row);

private:
    Ui::UnitForm *ui;
    NewPropertyForm *pNewPropertyForm;
    Unit mUnit;
    void initUnitsTable();
    void clearForm();
    void appendRecord(const UnitProperty &_property);
    void fillPropertyTable(const Unit &_unit);
    void clearTableWidget();
};

#endif // UNITFORM_H
