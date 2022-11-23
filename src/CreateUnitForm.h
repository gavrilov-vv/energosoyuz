#ifndef CREATEUNITFORM_H
#define CREATEUNITFORM_H

#include <QWidget>
#include "global_defs.h"
#include "Models.h"
#include "NewPropertyForm.h"

namespace Ui {
class CreateUnitForm;
}

class CreateUnitForm : public QWidget
{
    Q_OBJECT

public:
    explicit CreateUnitForm(QWidget *parent = nullptr);
    ~CreateUnitForm();


signals:
    void createUnit(Unit _unit);
    void updateUnit(Unit unit);

public slots:
    void onOpenCreateForm();
    void onOpenUpdateForm(Unit unit);


private slots:


    void on_btnCreateProperty_clicked();

    void on_btnDeleteProperty_clicked();

    void on_btnSaveUnit_clicked();

    void on_btnCloseForm_clicked();

    void onCreateUnitProperty(UnitProperty _property);

private:
    Ui::CreateUnitForm *ui;
    NewPropertyForm *pNewPropertyForm;
    Unit mUnit;
    void initUnitsTable();
    void clearForm();
    void appendRecord(const UnitProperty &_property);
    void fillPropertyTable(const Unit &_unit);
    void clearTableWidget();
};

#endif // CREATEUNITFORM_H
