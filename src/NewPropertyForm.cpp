#include "NewPropertyForm.h"
#include "ui_NewPropertyForm.h"

NewPropertyForm::NewPropertyForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewPropertyForm)
{
    ui->setupUi(this);

    ui->comboBox->addItem("Выбрать",static_cast<int>(PropertyType::UNDEF));
    ui->comboBox->addItem("Тип-1",static_cast<int>(PropertyType::TYPE_1));
    ui->comboBox->addItem("Тип-2", static_cast<int>(PropertyType::TYPE_2));
    ui->comboBox->addItem("Тип-3", static_cast<int>(PropertyType::TYPE_3));
    ui->comboBox->addItem("Тип-4", static_cast<int>(PropertyType::TYPE_4));
    ui->comboBox->addItem("Тип-5", static_cast<int>(PropertyType::TYPE_5));

}

NewPropertyForm::~NewPropertyForm()
{
    clearForm();
    delete ui;
}

void NewPropertyForm::on_btnCloseForm_clicked()
{
    clearForm();
    this->close();
}

void NewPropertyForm::on_btnCreateProperty_clicked()
{
    UnitProperty _property;

    _property.type = static_cast<PropertyType>(ui->comboBox->currentData().toInt());

    switch (_property.type) {
    case PropertyType::TYPE_1:
    case PropertyType::TYPE_2:
    case PropertyType::TYPE_3:
    case PropertyType::TYPE_4:
    case PropertyType::TYPE_5:
        _property.value = ui->lineEdit->text();
        if(!_property.value.isEmpty())
        {
            emit createNewProperty(_property);
            clearForm();
            this->close();
        }
        else
        {
            QMessageBox::critical(this,
                                  "Ошибка",
                                  "Поле \"Значение\" не может быть пустым");
        }
        break;
    default:
        QMessageBox::critical(this,
                              "Ошибка",
                              "Необходимо выбрать тип");
        break;
    }

}

void NewPropertyForm::clearForm()
{
    ui->comboBox->setCurrentIndex(0);
    ui->lineEdit->setText("");

}
