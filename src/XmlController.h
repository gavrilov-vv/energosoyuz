#ifndef XMLCONTROLLER_H
#define XMLCONTROLLER_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QtXml>
#include <QFile>
#include <QDebug>

#include "global_defs.h"
#include "Models.h"

class XmlController : public IController
{
    Q_OBJECT
public:
    explicit XmlController(QObject *parent = nullptr);
    ~XmlController();

    QString unitToXML(const Unit &unit);

    QList<UnitProperty> fetchProperties(const QString &_xmlString);

    Unit fetchUnit(const QString &_xmlString);

    int uploadToFile(QString _filePath, QList<Unit> _units);

    QList<Unit> loadFromFile(QString _pathXmlFile);

    // IController interface
public:
    int init(const Configurator *_config) override;

};

#endif // XMLCONTROLLER_H
