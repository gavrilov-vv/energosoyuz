#include "XmlController.h"

XmlController::XmlController(QObject *parent) :
    IController(parent)
{

}

XmlController::~XmlController()
{

}

QString XmlController::unitToXML(const Unit &unit)
{
    QString _xmlString;

    QXmlStreamWriter xmlWriter(&_xmlString);
    xmlWriter.setAutoFormatting(true);        // Устанавливаем автоформатирование текста
    xmlWriter.writeStartDocument();           // Запускаем запись в документ
    xmlWriter.writeStartElement("unit");      // Записываем первый элемент с его именем
    xmlWriter.writeAttribute("name",unit.name);

    for(UnitProperty _param : unit.properties)
    {
        xmlWriter.writeStartElement("property"); // Записываем первый элемент с его именем
        xmlWriter.writeAttribute("type", QString::number(static_cast<int>(_param.type)));
        xmlWriter.writeCharacters(_param.value);
        xmlWriter.writeEndElement();
    }

    // Закрываем тег "unit"
    xmlWriter.writeEndElement();

    // Завершаем запись в документ
    xmlWriter.writeEndDocument();

    return _xmlString;
}

QList<UnitProperty> XmlController::fetchProperties(const QString &_xmlString)
{
    QDomDocument domDoc;

    QList<UnitProperty> _lst;

    if(domDoc.setContent(_xmlString))
    {
        QDomElement domElement = domDoc.documentElement();

        if(domElement.toElement().tagName() == "unit")
        {
            QDomNodeList _items = domElement.elementsByTagName("property");

            for(int i=0; i < _items.size(); i++)
            {
                QDomNode _node = _items.at(i);

                if(_node.isElement())
                {
                    UnitProperty _prop;
                    _prop.type = static_cast<PropertyType>(_node.toElement().attribute("type", "").toInt());
                    _prop.value = _node.toElement().text();

                    _lst.append(_prop);

                }
            }
        }

    }

    return _lst;
}

Unit XmlController::fetchUnit(const QString &_xmlString)
{
    QDomDocument domDoc;

    Unit _unit;

    if(domDoc.setContent(_xmlString))
    {
        QDomElement domElement = domDoc.documentElement();

        if(domElement.toElement().tagName() == "unit")
        {
            _unit.id = domElement.attribute("unitId", "").toInt();
            _unit.name = domElement.attribute("name", "");

            QDomNodeList _items = domElement.elementsByTagName("property");

            for(int i=0; i < _items.size(); i++)
            {
                QDomNode _node = _items.at(i);
                if(_node.isElement())
                {
                    UnitProperty _prop;
                    _prop.type = static_cast<PropertyType>(_node.toElement().attribute("type", "").toInt());
                    _prop.value = _node.toElement().text();
                    _unit.properties.append(_prop);
                }
            }
        }
    }

    return _unit;
}

int XmlController::uploadToFile(QString _filePath, QList<Unit> _units)
{
    QFile _file(_filePath);
    if(!_file.open(QIODevice::WriteOnly))
    {
        return ResultCode::OPEN_FILE_ERROR;
    }

    QXmlStreamWriter xmlWriter(&_file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("units");
    for (auto unit : _units)
    {
        xmlWriter.writeStartElement("unit");      // Записываем первый элемент с его именем
        xmlWriter.writeAttribute("name",unit.name);

        for(UnitProperty _param : unit.properties)
        {
            xmlWriter.writeStartElement("property"); // Записываем первый элемент с его именем
            xmlWriter.writeAttribute("type", QString::number(static_cast<int>(_param.type)));
            xmlWriter.writeCharacters(_param.value);
            xmlWriter.writeEndElement();
        }

        // Закрываем тег "unit"
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();
    // Завершаем запись в документ
    xmlWriter.writeEndDocument();

    _file.close();

    return ResultCode::ALL_RIGHT;
}

QList<Unit> XmlController::loadFromFile(QString _pathXmlFile)
{
    QList<Unit> _lst;

    QFile _file(_pathXmlFile);

    if(!_file.exists())
    {

        throw Exception(ResultCode::FILE_NOT_FOUND,
                        "Файл не найден");

    }

    if(!_file.open(QFile::ReadOnly))
    {
        throw Exception(ResultCode::OPEN_FILE_ERROR,
                        "Не возможно открыть файл");
    }

    QByteArray _data = _file.readAll();

    QDomDocument domDoc;

    if(domDoc.setContent(_data))
    {
        QDomElement domElement = domDoc.documentElement();

        QDomNodeList units= domElement.elementsByTagName("unit");

        for(int i=0; i < units.size(); i++)
        {
            Unit _unit;

            QDomNode _node = units.at(i);

            _unit.name = _node.toElement().attribute("name", "");

            QDomNodeList properties = _node.toElement().elementsByTagName("property");

            for(int j=0; j < properties.size(); j++)
            {
                QDomNode _node1 = properties.at(j);

                UnitProperty _prop;

                _prop.type = static_cast<PropertyType>(_node1.toElement().attribute("type", "").toInt());

                _prop.value = _node1.toElement().text();

                _unit.properties.append(_prop);
            }

            _lst.append(_unit);

        }

    }

    return _lst;
}


int XmlController::init(const Configurator *_config)
{
    Q_UNUSED(_config)

    return ResultCode::ALL_RIGHT;
}
