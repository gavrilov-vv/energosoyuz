#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include <QCoreApplication>
#include <QProcess>

#include "global_defs.h"
#include "Configurator.h"


class UnitProperty
{
public:
    UnitProperty();

    PropertyType type;
    QString value;

};

class Unit
{
public:
    Unit() :
        id(0),
        name("UNDEF")
    {

    }

    int id;
    QString name;
    QList<UnitProperty> properties;

    bool operator ==(const Unit &_unit)
    {
        return id == _unit.id;
    }

    bool operator !=(const Unit &_unit)
    {
        return id != _unit.id;
    }

    void reset()
    {
        id = 0;
        name = "UNDEF";
        properties.clear();
    }
};

/*!
 * \brief Класс исключений
 */
class Exception
{
public:
    Exception(int _err, QString _descr);

    int err;       /*!< Код исключения */
    QString descr; /*!< Описание исключения */

};
/*!
 * \brief Базовый класс для контроллеров
 */
class IController : public QObject
{
    Q_OBJECT
public:
    IController(QObject *parent = nullptr);
    virtual ~IController();
    /*!
     * \brief Инициализация
     * \param Указатель _app
     * \return
     */
    virtual int init(const Configurator *_config) = 0;

signals:
    void message(const QString &_msg, MsgType _type);

protected slots:
    /*!
     * \brief Обработчик сигнала message
     * \param _msg - Сообщения
     * \param _type - Тип сообщения
     */
    virtual void onMessage(QString _msg, MsgType _type);

protected:
    /*!
     * \brief Вызов и выполнение системной команды
     * \param _cmd - Команда
     * \return
     */
    virtual QString runSystemCommand(const QString &_cmd);

};

/*!
 * \brief Базовый класс для контроллеров,
 * работающих в отдельных вычислительных потоках
 */
class IThreadController : public IController
{
    Q_OBJECT
public:
    IThreadController(QObject *parent);
    virtual ~IThreadController();

signals:
    void finished();

protected slots:
    virtual void onStartThread() = 0;
};


#endif // MODELS_H
