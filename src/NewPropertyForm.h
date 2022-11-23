#ifndef NEWPROPERTYFORM_H
#define NEWPROPERTYFORM_H

#include <QWidget>
#include <QMessageBox>

#include "global_defs.h"
#include "Models.h"

namespace Ui {
class NewPropertyForm;
}

class NewPropertyForm : public QWidget
{
    Q_OBJECT

public:
    explicit NewPropertyForm(QWidget *parent = nullptr);
    ~NewPropertyForm();

private slots:
    void on_btnCloseForm_clicked();

    void on_btnCreateProperty_clicked();

signals:
    void createNewProperty(UnitProperty _property);

private:
    Ui::NewPropertyForm *ui;

    void clearForm();
};

#endif // NEWPROPERTYFORM_H
