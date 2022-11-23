#include "Configurator.h"

Configurator::Configurator(QObject *parent) :
    QObject(parent),
    pSettings{nullptr},
    mInitStatus(false)
{

}

Configurator::~Configurator()
{
    if(pSettings) delete pSettings;
}

int Configurator::init(QString &_appDirPath)
{
    int _result = ResultCode::UNDEF;

    QString _fileName = _appDirPath.append("/energo.conf");

    QFile _f(_fileName);

    if(_f.exists())
    {
        pSettings = new QSettings(_fileName, QSettings::IniFormat);
        mInitStatus = true;
        _result = ResultCode::ALL_RIGHT;
    }
    else
    {
        _result = ResultCode::FILE_NOT_FOUND;
    }


    return _result;
}

QString Configurator::getDbAddr() const
{
    QString _value = "UNDEF";

    if(pSettings)
    {
        pSettings->beginGroup("DB");
        _value = pSettings->value("address", "UNDEF").toString();
        pSettings->endGroup();
    }

    return _value;
}

QString Configurator::getDbUser() const
{
    QString _value = "UNDEF";

    if(pSettings)
    {
        pSettings->beginGroup("DB");
        _value = pSettings->value("user", "UNDEF").toString();
        pSettings->endGroup();
    }

    return _value;
}

QString Configurator::getDbName() const
{
    QString _value = "UNDEF";

    if(pSettings)
    {
        pSettings->beginGroup("DB");
        _value = pSettings->value("name", "UNDEF").toString();
        pSettings->endGroup();
    }

    return _value;
}

QString Configurator::getDbPassword() const
{
    QString _value = "UNDEF";

    if(pSettings)
    {
        pSettings->beginGroup("DB");
        _value = pSettings->value("password", "UNDEF").toString();
        pSettings->endGroup();
    }

    return _value;
}

int Configurator::getDbPort() const
{
    int _value = 5432;

    if(pSettings)
    {
        pSettings->beginGroup("DB");
        _value = pSettings->value("port", 5432).toInt();
        pSettings->endGroup();
    }

    return _value;
}



