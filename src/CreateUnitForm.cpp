#include "CreateUnitForm.h"
#include "ui_CreateUnitForm.h"

CreateUnitForm::CreateUnitForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateUnitForm),
    pNewPropertyForm(new NewPropertyForm())
{
    ui->setupUi(this);

    connect(pNewPropertyForm, SIGNAL(createNewProperty(UnitProperty)),
            this, SLOT(onCreateUnitProperty(UnitProperty)));

    initUnitsTable();
}

CreateUnitForm::~CreateUnitForm()
{
    delete ui;
}


void CreateUnitForm::on_btnCreateProperty_clicked()
{
    pNewPropertyForm->show();
}

void CreateUnitForm::on_btnDeleteProperty_clicked()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void CreateUnitForm::on_btnSaveUnit_clicked()
{
    Unit _unit;

    _unit.name = ui->lineEdit->text();

    int rows = ui->tableWidget->rowCount();

    for(int i = 0; i < rows; i++)
    {
        UnitProperty _property;

        _property.type = static_cast<PropertyType>(ui->tableWidget->model()->index(i, 0).data().toInt());
        _property.value = ui->tableWidget->model()->index(i, 2).data().toString();

        _unit.properties.append(_property);
    }

    if(ui->btnSaveUnit->text() == "Создать")
    {
        emit createUnit(_unit);
    }
    else if(ui->btnSaveUnit->text() == "Изменить")
    {
        _unit.id = mUnit.id;
        emit updateUnit(_unit);
    }
    else
    {
        qDebug() << ui->btnSaveUnit->text();

    }

    clearForm();

    this->close();
}

void CreateUnitForm::on_btnCloseForm_clicked()
{
    clearForm();
    this->close();
}

void CreateUnitForm::onCreateUnitProperty(UnitProperty _property)
{
    appendRecord(_property);
}

void CreateUnitForm::initUnitsTable()
{
    clearTableWidget();

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setCornerButtonEnabled(true);

    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "typeId"
                                                             << "Тип"
                                                             << "Значение");
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setMinimumSectionSize(100);

    ui->tableWidget->hideColumn(0);

}

void CreateUnitForm::onOpenCreateForm()
{
    ui->btnSaveUnit->setText("Создать");
    initUnitsTable();
    this->show();
}

void CreateUnitForm::onOpenUpdateForm(Unit unit)
{
    clearForm();
    mUnit = unit;
    this->setWindowTitle("Изменить информацию");
    ui->lineEdit->setText(unit.name);
    initUnitsTable();
    fillPropertyTable(unit);

    ui->btnSaveUnit->setText("Изменить");
    this->show();
}

void CreateUnitForm::clearForm()
{
    clearTableWidget();
    ui->lineEdit->setText("");
    ui->btnSaveUnit->setText("UNDEF");
}

void CreateUnitForm::fillPropertyTable(const Unit &_unit)
{
    initUnitsTable();

    for(auto _p : _unit.properties)
    {
        appendRecord(_p);
    }
}


void CreateUnitForm::appendRecord(const UnitProperty &_property)
{
    int rowNumber = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowNumber);

    QString _typeStr;

    switch (_property.type) {
    case PropertyType::UNDEF:
        _typeStr = "UNDEF";
        break;
    case PropertyType::TYPE_1:
        _typeStr = "ТИП 1";
        break;
    case PropertyType::TYPE_2:
        _typeStr = "ТИП 2";
        break;
    case PropertyType::TYPE_3:
        _typeStr = "ТИП 3";
        break;
    case PropertyType::TYPE_4:
        _typeStr = "ТИП 4";
        break;
    case PropertyType::TYPE_5:
        _typeStr = "ТИП 5";
        break;
    default:
        break;
    }

    int _p = static_cast<int>(_property.type);

    ui->tableWidget->setItem(rowNumber,
                             0,
                             new QTableWidgetItem(QString::number(_p)));

    QTableWidgetItem *item = new QTableWidgetItem();
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setText(_typeStr);
    ui->tableWidget->setItem(rowNumber, 1, item);

    ui->tableWidget->setItem(rowNumber,
                             2,
                             new QTableWidgetItem(_property.value));

}

void CreateUnitForm::clearTableWidget()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
}
