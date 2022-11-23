#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QPushButton>

#include <memory>

#include "global_defs.h"
#include "Models.h"
#include "Configurator.h"
#include "DbController.h"
#include "XmlController.h"
#include "UnitForm.h"
#include "ToolButton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int init();

signals:
    void openCreateForm();
    void openUpdateForm(Unit unit);
    void uploadDataToXML(QString _filePath);
    void loadDataFromXmlFile(QString _filePath);
    void deleteUnit(Unit _unit);

private slots:

    void on_btnUploadXML_clicked();

    void onMessage(const QString _msg, MsgType _type);

    void on_btnChangeUnit_clicked();

    void on_btnShowFormCreateUnit_clicked();

    void onUpdateDataSet(QList<Unit> _lst);

    void on_btnLoadDataFromXmlFile_clicked();

    void on_btnDeleteUnit_clicked();

private:
    Ui::MainWindow *ui;
    Configurator *pConfig;
    QThread *pThread;
    DbController *pDbController;
    XmlController *pXmlController;
    UnitForm *pUnitForm;
    QList<Unit> units;

    Unit findUnitById(const QList<Unit> &_lst, quint32 _id);


};

Q_DECLARE_METATYPE(Unit)
Q_DECLARE_METATYPE(QList<Unit>)
Q_DECLARE_METATYPE(UnitProperty)
Q_DECLARE_METATYPE(QList<UnitProperty>)
Q_DECLARE_METATYPE(MsgType)

#endif // MAINWINDOW_H
