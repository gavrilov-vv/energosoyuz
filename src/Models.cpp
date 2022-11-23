#include "Models.h"

IController::IController(QObject *parent) :
    QObject(parent)
{

}

IController::~IController()
{

}

void IController::onMessage(QString _msg, MsgType _type)
{
    emit message(_msg, _type);
}

QString IController::runSystemCommand(const QString &_cmd)
{
    QProcess _proc;
    QString result;

    // Вызвать системную команду
    _proc.start("/bin/bash", QStringList() << "-c" << _cmd);

    // Ждать до окончания работы процесса
    _proc.waitForFinished(-1);

    // Вернуть стандартный вывод команды
    result = QString(_proc.readAllStandardOutput());

    int _exitCode = _proc.exitCode();

    if(_exitCode != ResultCode::ALL_RIGHT)
    {
        throw Exception(ResultCode::EXEC_ERROR,
                        QString("Ошибка исполнения: %1. result=%2 exitCode=%3")
                            .arg(_cmd)
                            .arg(result.trimmed())
                            .arg(_exitCode));
    }

    return result;
}

Exception::Exception(int _err, QString _descr)
{
    err = _err;
    descr = _descr;
}

IThreadController::IThreadController(QObject *parent) : IController(parent)
{

}

IThreadController::~IThreadController()
{

}

UnitProperty::UnitProperty() :
    type(PropertyType::UNDEF),
    value("UNDEF")
{

}
