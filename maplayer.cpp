#include "maplayer.h"

#include <QUrl>

const QString &MapLayer::getTitle() const
{
    return title;
}

void MapLayer::setTitle(const QString &newTitle)
{
    title = newTitle;
}

const QString &MapLayer::getID() const
{
    return ID;
}

void MapLayer::setID(const QString &newID)
{
    ID = newID;
}

const QString &MapLayer::getFormat() const
{
    return format;
}

void MapLayer::setFormat(const QString &newFormat)
{
    format = newFormat;
}

const QString &MapLayer::getTileMatrixSet() const
{
    return tileMatrixSet;
}

void MapLayer::setTileMatrixSet(const QString &newTileMatrixSet)
{
    tileMatrixSet = newTileMatrixSet;
}

const QString &MapLayer::getTemplate() const
{
    return _template;
}

void MapLayer::setTemplate(const QString &newTemplate)
{
    _template = newTemplate;
}

QDate *MapLayer::getMinDate() const
{
    return minDate;
}

void MapLayer::setMinDate(QDate *newMinDate)
{
    minDate = newMinDate;
}

QDate *MapLayer::getMaxDate() const
{
    return maxDate;
}

void MapLayer::setMaxDate(QDate *newMaxDate)
{
    maxDate = newMaxDate;
}

int MapLayer::getLowerLat() const
{
    return lowerLat;
}

void MapLayer::setLowerLat(int newLowerLat)
{
    lowerLat = newLowerLat;
}

int MapLayer::getLowerLon() const
{
    return lowerLon;
}

void MapLayer::setLowerLon(int newLowerLon)
{
    lowerLon = newLowerLon;
}

int MapLayer::getUpperLat() const
{
    return upperLat;
}

void MapLayer::setUpperLat(int newUpperLat)
{
    upperLat = newUpperLat;
}

int MapLayer::getUpperLon() const
{
    return upperLon;
}

void MapLayer::setUpperLon(int newUpperLon)
{
    upperLon = newUpperLon;
}

QDate *MapLayer::getDefaultDate() const
{
    return defaultDate;
}

void MapLayer::setDefaultDate(QDate *newDefaultDate)
{
    defaultDate = newDefaultDate;
}

MapLayer::MapLayer(QString title,
                   QString ID,
                   QString format,
                   QString tileMatrixSet,
                   QString _template,
                   QDate* minDate,
                   QDate* maxDate,
                   QDate* defaultDate,
                   int lowerLat,
                   int lowerLon,
                   int upperLat,
                   int upperLon)
    : title(std::move(title)),
      ID(std::move(ID)),
      format(std::move(format)),
      tileMatrixSet(std::move(tileMatrixSet)),
      _template(std::move(_template)),
      minDate(minDate),
      maxDate(maxDate),
      defaultDate(defaultDate),
      lowerLat(lowerLat),
      lowerLon(lowerLon),
      upperLat(upperLat),
      upperLon(upperLon)
{

}

MapLayer::~MapLayer() {
    delete minDate, delete maxDate, delete defaultDate;
}

MapLayer::MapLayer() {
    minDate = nullptr, maxDate = nullptr, minDate = nullptr, lowerLat = 0, lowerLon = 0, upperLat = 0, upperLon = 0;
}

const bool MapLayer::replace(std::string& str, const std::string& from, const std::string& to) const {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

// This will follow the template - does not check the validity of the URL request
const QUrl MapLayer::getTileUrl(int row, int column, int zoom, QDate date) const {
    std::string url = _template.toStdString();
    replace(url, std::string("{LayerIdentifier}"), ID.toStdString());
    replace(url, std::string("{Time}"), date.toString("yyyy-MM-dd").toStdString());
    replace(url, std::string("{TileMatrixSet}"), tileMatrixSet.toStdString());
    replace(url, std::string("{TileMatrix}"), std::to_string(zoom));
    replace(url, std::string("{TileRow}"), std::to_string(row));
    replace(url, std::string("{TileCol}"), std::to_string(column));
    replace(url, std::string("{FormatExt}"), format.toStdString().substr(0, format.toStdString().find("/")));
    replace(url, std::string("{FormatMime}"), format.toStdString().substr(0, format.toStdString().find("/")));
    return {QString::fromStdString(std::move(url))};
}
