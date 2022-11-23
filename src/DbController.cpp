#include "DbController.h"

DbController::DbController(QObject *parent) :
    IThreadController(parent),
    mConnectionName("UNDEF"),
    mDbName("UNDEF"),
    mDbAddress("UNDEF"),
    mDbPort(0),
    mDbUser("UNDEF"),
    mDbPassword("UNDEF"),
    xmlController(new XmlController(this))
{

}

DbController::~DbController()
{

}

void DbController::onCreateUnit(Unit unit)
{

    QSqlDatabase dbh = QSqlDatabase::database(mConnectionName);

    if(!dbh.isOpen())
    {
        emit message("Не могу подключиться к БД",
                     MsgType::ERROR);
    }

    QSqlQuery _psqlQuery(dbh);
    qint32 _id = 0;
    QString _sql_request = QString("INSERT INTO units ("
                                       "name,"
                                       "properties"
                                   ") "
                                   "VALUES "
                                   "("
                                       ":name,"
                                       ":properties"
                                   ") RETURNING id;");

    if(_psqlQuery.prepare(_sql_request))
    {
        _psqlQuery.bindValue(":name", unit.name);
        QString _propStr = xmlController->unitToXML(unit);
        _psqlQuery.bindValue(":properties", _propStr);

        if(!_psqlQuery.exec())
        {
            emit message(_psqlQuery.lastError().text(),
                         MsgType::ERROR);
        }

        _id = _psqlQuery.lastInsertId().toInt();


    }
    else
    {
        emit message(_psqlQuery.lastError().text(),
                     MsgType::ERROR);
    }

    // Выборка из таблицы устройств
    QList<Unit> _lst = getAllUnits();
    // Обновление ГПИ
    emit updateDataSet(_lst);

}

void DbController::onUnitChanged(Unit _unit)
{
    // TODO

    QSqlDatabase dbh = QSqlDatabase::database(mConnectionName);

    if(dbh.isOpenError())
    {
        emit message("Не могу подключиться к БД",
                     MsgType::ERROR);
        return;
    }

    QSqlQuery _psqlQuery(dbh);

    QString _sql_request = "UPDATE "
                               "units "
                           "SET "
                               "name= :name, "
                               "properties=:properties "
                           "WHERE id=%1;";

    if(_psqlQuery.prepare(_sql_request.arg(_unit.id)))
    {
        _psqlQuery.bindValue(":name", _unit.name);

        QString _xml = xmlController->unitToXML(_unit);

        _psqlQuery.bindValue(":properties", _xml);


        if(!_psqlQuery.exec())
        {
            emit message(_psqlQuery.lastError().text(),
                         MsgType::ERROR);
        }
    }
    else
    {
        emit message(_psqlQuery.lastError().text(),
                     MsgType::ERROR);
    }


    // Выборка из таблицы устройств
    QList<Unit> _lst = getAllUnits();
    // Обновление ГПИ
    emit updateDataSet(_lst);
}

void DbController::onDeleteUnit(Unit _unit)
{

    QSqlDatabase dbh = QSqlDatabase::database(mConnectionName);

    if(dbh.isOpenError())
    {
        emit message("Не могу подключиться к БД",
                     MsgType::ERROR);
        return;
    }

    QSqlQuery _psqlQuery(dbh);

    QString _sql_request = "DELETE FROM units WHERE id=%1";

    if(_psqlQuery.exec(_sql_request.arg(_unit.id)))
    {
        // Выборка из таблицы устройств
        QList<Unit> _lst = getAllUnits();
        // Обновление ГПИ
        emit updateDataSet(_lst);
    }
    else
    {
        emit message(_psqlQuery.lastError().text(),
                     MsgType::ERROR);
    }
}

void DbController::onUploadDataToXML(QString _filePath)
{
    QList<Unit> _lst = getAllUnits();

    if(!_lst.isEmpty())
    {
        int _result = xmlController->uploadToFile(_filePath, _lst);

        if(_result != ResultCode::ALL_RIGHT)
        {
            emit message("Ошибка выгрузки файла XML",
                         MsgType::ERROR);
        }
        else
        {
            emit message("Выгрузка данных в XML выполнена.",
                         MsgType::INFO);
        }
    }
    else
    {
        emit message("Нет данных для выгрузки. Список устройств пуст",
                     MsgType::INFO);
    }

    // Обновление ГПИ
    emit updateDataSet(_lst);
}

void DbController::onLoadDataFromXmlFile(QString _fileName)
{
    try
    {
        QList<Unit> _lst = xmlController->loadFromFile(_fileName);

        QSqlDatabase dbh = QSqlDatabase::database(mConnectionName);

        if(!dbh.isOpen())
        {
            emit message("Не могу подключиться к БД. "
                         "Загрузка данных из XML не выполнена!",
                         MsgType::ERROR);
            return;
        }

        QSqlQuery _psqlQuery(dbh);

        QString _sql_request = "DELETE FROM units;";

        if(!_psqlQuery.exec(_sql_request))
        {
            QString _errStr = QString("Ошибка очистки таблицы. "
                                      "Загрузка данных из XML "
                                      "не выполнена. %1")
                                  .arg(_psqlQuery.lastError().text());
            emit message(_errStr, MsgType::ERROR);
            return;
        }

        qint32 _id = 0;
        _sql_request = QString("INSERT INTO units ("
                                       "name,"
                                       "properties"
                                       ") "
                                       "VALUES "
                                       "("
                                       ":name,"
                                       ":properties"
                                       ") RETURNING id;");

        if(_psqlQuery.prepare(_sql_request))
        {
            foreach (Unit _u, _lst)
            {
                _psqlQuery.bindValue(":name", _u.name);
                QString _propStr = xmlController->unitToXML(_u);
                _psqlQuery.bindValue(":properties", _propStr);

                if(!_psqlQuery.exec())
                {
                    emit message(_psqlQuery.lastError().text(),
                                 MsgType::ERROR);
                }

                _id = _psqlQuery.lastInsertId().toInt();
            }

        }
        else
        {
            emit message(_psqlQuery.lastError().text(),
                         MsgType::ERROR);
        }

        emit message("Загрузка данных из XML выполнена.",
                     MsgType::INFO);


    } catch (Exception e)
    {
        emit message(e.descr, MsgType::ERROR);
    }


    // Выборка из таблицы устройств
    QList<Unit> _lst = getAllUnits();
    // Обновление ГПИ
    emit updateDataSet(_lst);

}

QList<Unit> DbController::getAllUnits()
{
    QList<Unit> _lst;

    QSqlDatabase dbh = QSqlDatabase::database(mConnectionName);

    if(!dbh.isOpen())
    {
        throw Exception(ResultCode::DB_OPEN_ERROR,
                        dbh.lastError().text());
    }

    QString _sql_request = QString("SELECT "
                                       "id, "
                                       "name, "
                                       "properties "
                                   "FROM "
                                       "public.units "
                                   "ORDER BY id;");

    QSqlQuery _psqlQuery(dbh);

    if(!_psqlQuery.exec(_sql_request))
    {
        throw Exception(ResultCode::DB_QUERY_ERROR,
                        _psqlQuery.lastError().text());
    }
    else
    {
        while(_psqlQuery.next())
        {
            Unit _unit;

            _unit.id = _psqlQuery.value("id").toInt();
            _unit.name = _psqlQuery.value("name").toString();
            QString _propList = _psqlQuery.value("properties").toString();
            _unit.properties = xmlController->fetchProperties(_propList);

            _lst.append(_unit);

        }
    }

    return _lst;
}

int DbController::init(const Configurator *_config)
{
    int _res = xmlController->init(_config);

    if(_res != ResultCode::ALL_RIGHT)
    {
        return _res;
    }

    if(_config->getDbUser().isEmpty() || _config->getDbName().isEmpty() || _config->getDbAddr().isEmpty())
    {
        return ResultCode::INIT_ERROR;
    }

    mConnectionName = "energo";
    mDbName = _config->getDbName();
    mDbPassword = _config->getDbPassword();
    mDbUser = _config->getDbUser();
    mDbAddress = _config->getDbAddr();
    mDbPort = _config->getDbPort();

    _res = createScheme(mDbAddress, mDbPort, mDbUser, mDbPassword);

    return _res;
}

int DbController::createScheme(QString _addr, qint32 _port, QString _user, QString _password)
{
    { // Эти скобки обязательны для правильного удаления дескриптора
        QSqlDatabase dbh;

        if(QSqlDatabase::contains("admConnection"))
        {
            dbh = QSqlDatabase::database("admConnection");
        }
        else
        {
            dbh = QSqlDatabase::addDatabase("QPSQL", "admConnection");
            dbh.setDatabaseName("postgres");
            dbh.setHostName(_addr);
            dbh.setPort(_port);
            dbh.setUserName(_user);
            dbh.setPassword(_password);
            //Таймаут подключения = 3 сек
            dbh.setConnectOptions("connect_timeout=3");
        }

        if(!dbh.open())
        {
            emit message(dbh.lastError().text(), MsgType::ERROR);
            QSqlDatabase::removeDatabase("admConnection");
            return ResultCode::INIT_ERROR;
        }

        QSqlQuery query(dbh);

        QString _queryStr = QString("CREATE DATABASE %1 "
                                    "OWNER=postgres "
                                    "ENCODING='UTF8' "
                                    "TABLESPACE=pg_default "
                                    "LC_COLLATE='ru_RU.UTF-8' "
                                    "LC_CTYPE='ru_RU.UTF-8' "
                                    "CONNECTION LIMIT = -1;").arg(mDbName);

        if(query.exec(_queryStr))
        {
            qDebug() << "Успешно создана новая БД";
        }

        query.clear();
        dbh.close();

        dbh.setDatabaseName(mDbName);
        dbh.open();

        _queryStr = "CREATE TABLE IF NOT EXISTS units("
                            "id SERIAL PRIMARY KEY, "
                            "name TEXT NOT NULL, "
                            "properties TEXT NOT NULL);";

        if(query.exec(_queryStr) == false)
        {
            qDebug() << query.lastError().text();
            return ResultCode::DB_QUERY_ERROR;
        }

        query.clear();
        dbh.close();
    }

    QSqlDatabase::removeDatabase("admConnection");

    return ResultCode::ALL_RIGHT;
}


void DbController::onStartThread()
{

    { // Эти скобки обязательны для правильного удаления дескриптора
        QSqlDatabase dbh;

        if(QSqlDatabase::contains(mConnectionName))
        {
            dbh = QSqlDatabase::database(mConnectionName);
        }
        else
        {
            dbh = QSqlDatabase::addDatabase("QPSQL", mConnectionName);
            dbh.setDatabaseName(mDbName);
            dbh.setHostName(mDbAddress);
            dbh.setPort(mDbPort);
            dbh.setUserName(mDbUser);
            dbh.setPassword(mDbPassword);
            //Таймаут подключения = 3 сек
            dbh.setConnectOptions("connect_timeout=3");
        }

        if(!dbh.open())
        {
            emit message(dbh.lastError().text(), MsgType::ERROR);
            QSqlDatabase::removeDatabase(mConnectionName);

        }
    }

    // Выборка из таблицы устройств
    QList<Unit> _lst = getAllUnits();
    // Обновление ГПИ
    emit updateDataSet(_lst);

}
