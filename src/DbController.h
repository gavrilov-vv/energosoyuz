#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <QObject>
#include <QHostAddress>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>

#include "global_defs.h"
#include "Models.h"
#include "XmlController.h"

class DbController : public IThreadController
{
    Q_OBJECT
public:
    explicit DbController(QObject *parent = nullptr);
    ~DbController();

    // IController interface
public:
    int init(const Configurator *_config) override;

signals:
    void updateDataSet(QList<Unit> _lst);

public slots:

    void onCreateUnit(Unit _unit);
    void onUnitChanged(Unit _unit);
    void onDeleteUnit(Unit _unit);
    void onUploadDataToXML(QString _filePath);
    void onLoadDataFromXmlFile(QString _fileName);

private:

    int createScheme(QString _addr, qint32 _port, QString _user, QString _password);
    QList<Unit> getAllUnits();
    QString mConnectionName;   /*!< Имя подключения к БД */
    QString mDbName;           /*!< Имя БД */
    QString mDbAddress;        /*!< Адрес сервера СУБД */
    qint32  mDbPort;           /*!< Порт сервера СУБД */
    QString mDbUser;           /*!< Учетная запись пользователя БД */
    QString mDbPassword;       /*!< Пароль учетной записи */
    XmlController *xmlController;


    // IThreadController interface
protected slots:
    void onStartThread() override;
};

#endif // DBCONTROLLER_H
