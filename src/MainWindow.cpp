#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    pConfig(new Configurator(this)),
    pThread(new QThread()),
    pDbController(new DbController()),
    pXmlController(new XmlController(this)),
    pUnitForm(new UnitForm())
{
    // Регистрация метатипов
    qRegisterMetaType<Unit>();
    qRegisterMetaType<QList<Unit>>();
    qRegisterMetaType<UnitProperty>();
    qRegisterMetaType<QList<UnitProperty>>();
    qRegisterMetaType<MsgType>();



    ui->setupUi(this);

    ui->btnChangeUnit->setVisible(false);
    ui->btnDeleteUnit->setVisible(false);

}

MainWindow::~MainWindow()
{
    pXmlController->deleteLater();
    pThread->deleteLater();
    delete ui;
}

int MainWindow::init()
{

    connect(pThread, SIGNAL(started()),
            pDbController, SLOT(onStartThread()));

    connect(pThread, SIGNAL(finished()),
            pDbController, SLOT(deleteLater()));

    connect(pDbController, SIGNAL(finished()),
            pThread, SLOT(quit()));

    connect(pUnitForm, SIGNAL(createUnit(Unit)),
            pDbController, SLOT(onCreateUnit(Unit)));

    connect(pUnitForm, SIGNAL(unitChanged(Unit)),
            pDbController, SLOT(onUnitChanged(Unit)));

    connect(this, SIGNAL(deleteUnit(Unit)),
            pDbController, SLOT(onDeleteUnit(Unit)));

    connect(pDbController, SIGNAL(message(QString, MsgType)),
            this, SLOT(onMessage(QString, MsgType)));

    connect(pDbController, SIGNAL(updateDataSet(QList<Unit>)),
            this, SLOT(onUpdateDataSet(QList<Unit>)));

    connect(this, SIGNAL(openCreateForm()),
            pUnitForm, SLOT(onOpenCreateForm()));

    connect(this, SIGNAL(openUpdateForm(Unit)),
            pUnitForm, SLOT(onOpenUpdateForm(Unit)));

    connect(this, SIGNAL(uploadDataToXML(QString)),
            pDbController, SLOT(onUploadDataToXML(QString)));

    connect(this, SIGNAL(loadDataFromXmlFile(QString)),
            pDbController, SLOT(onLoadDataFromXmlFile(QString)));

    QString _dir = QApplication::applicationDirPath();

    int _result = pConfig->init(_dir);

    if(_result != ResultCode::ALL_RIGHT)
    {
        QMessageBox::critical(this,
                              "Critical error",
                              "Ошибка инициализации конфигуратора");
        return ResultCode::INIT_ERROR;
    }

    _result = pDbController->init(pConfig);

    if(_result != ResultCode::ALL_RIGHT)
    {
        QMessageBox::critical(this,
                              "Critical error",
                              "Ошибка инициализации контроллера базы данных");
        return _result;

    }

    _result = pXmlController->init(pConfig);

    if(_result != ResultCode::ALL_RIGHT)
    {
        QMessageBox::critical(this,
                              "Critical error",
                              "Ошибка инициализации контроллера XML");
        return _result;

    }

    pDbController->moveToThread(pThread);
    pThread->start();

    return ResultCode::ALL_RIGHT;
}

void MainWindow::on_btnUploadXML_clicked()
{
    QFileDialog dlg;

    QString dir = qApp->applicationDirPath();
    QString name = "test.xml";

    QString _path = QString("%1/%2").arg(dir, name);

    QString _filePath = dlg.getSaveFileName(this,
                                            tr("Выгрузить данные"),
                                            _path,
                                            tr("XML (*.xml)"));

    emit uploadDataToXML(_filePath);



}

void MainWindow::onMessage(const QString _msg, MsgType _type)
{
    if(_type == MsgType::ERROR)
    {
        QMessageBox::critical(this,
                                 "Ошибка",
                                 _msg);
    }
    else
    {
        QMessageBox::information(this,
                                 "Сообщение",
                                 _msg);
    }

}

void MainWindow::onUpdateDataSet(QList<Unit> _lst)
{
    // Обновить список
    units = _lst;

    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(6);
    ui->treeWidget->hideColumn(3);
    ui->treeWidget->header()->setVisible(false);


    QTreeWidgetItem *item1 = new QTreeWidgetItem();
    item1->setText(0, "Устройство");
    item1->setText(1, "Тип");
    item1->setText(2, "Значение");
    item1->setText(3, "Id");
    item1->setText(4, "Удалить");
    item1->setText(5, "Изменить");

    item1->setBackground(0, Qt::lightGray);
    item1->setBackground(1, Qt::lightGray);
    item1->setBackground(2, Qt::lightGray);
    item1->setBackground(3, Qt::lightGray);
    item1->setBackground(4, Qt::lightGray);
    item1->setBackground(5, Qt::lightGray);


    ui->treeWidget->addTopLevelItem(item1);

    if(!_lst.isEmpty())
    {
        for(auto _unit : _lst)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem();


            item->setText(3, QString::number(_unit.id));
            item->setText(0, _unit.name);

            ui->treeWidget->addTopLevelItem(item);

            foreach (UnitProperty prop, _unit.properties)
            {
                QTreeWidgetItem *child = new QTreeWidgetItem();

//                int yyy = static_cast<int>(prop.type);

                QString _typeStr;

                switch (prop.type) {
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

                child->setText(1, _typeStr);
                child->setText(2, prop.value);
                item->addChild(child);
            }

            ToolButton *tb = new ToolButton(_unit, OpType::DELETE);
            connect(tb, SIGNAL(deleteUnit(Unit)),
                    pDbController, SLOT(onDeleteUnit(Unit)));
            ui->treeWidget->setItemWidget(item, 4, tb);

            ToolButton *tb1 = new ToolButton(_unit, OpType::CHANGE);
            connect(tb1, SIGNAL(changeUnit(Unit)),
                    this, SIGNAL(openUpdateForm(Unit)));
            ui->treeWidget->setItemWidget(item, 5, tb1);

        }
    }
}

void MainWindow::on_btnChangeUnit_clicked()
{
    // TODO Тут функционал по извлечению данных выделенного устройства

    QString _strId = ui->treeWidget->currentItem()->text(3);

    if(!_strId.isEmpty())
    {
        try {
            Unit _unit = findUnitById(units, _strId.toInt());
            emit openUpdateForm(_unit);
        } catch (Exception e) {
            onMessage(e.descr, MsgType::ERROR);
        }

    }


}



void MainWindow::on_btnShowFormCreateUnit_clicked()
{
    emit openCreateForm();
}

void MainWindow::on_btnLoadDataFromXmlFile_clicked()
{
    QString dir = qApp->applicationDirPath();

    QString file1Name = QFileDialog::getOpenFileName(this,
                                                     tr("Открыть XML"),
                                                     dir,
                                                     tr("XML Files (*.xml)"));



    emit loadDataFromXmlFile(file1Name);
}

Unit MainWindow::findUnitById(const QList<Unit> &_lst, quint32 _id)
{
    Unit _unit;
    _unit.id = _id;

    if(_lst.contains(_unit))
    {
        int _idx = _lst.indexOf(_unit);
        _unit = _lst.at(_idx);
    }
    else
    {
        throw Exception(ResultCode::UNIT_NOT_FOUND,
                        "Устройство не найдено");
    }

    return _unit;
}

void MainWindow::on_btnDeleteUnit_clicked()
{
    QString _strId = ui->treeWidget->currentItem()->text(3);

    if(!_strId.isEmpty())
    {
        try {
            Unit _unit = findUnitById(units, _strId.toInt());
            emit deleteUnit(_unit);
        } catch (Exception e) {
            onMessage(e.descr, MsgType::ERROR);
        }

    }
}
