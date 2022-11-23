#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QObject>
#include <QSettings>
#include <QFile>
#include <QApplication>

#include "global_defs.h"

/*!
 * \brief Конфигуратор приложения
 *
 * Содержит методы для чтения конфигурационных параметров
 */
class Configurator : public QObject
{
    Q_OBJECT
public:
    explicit Configurator(QObject *parent = nullptr);
    ~Configurator();
    int init(QString &_appDirPath);

    QString getDbAddr() const;
    QString getDbUser() const;
    QString getDbName() const;
    QString getDbPassword() const;
    int getDbPort() const;

private:
    QSettings *pSettings;
    bool mInitStatus;
};

#endif // CONFIGURATOR_H
