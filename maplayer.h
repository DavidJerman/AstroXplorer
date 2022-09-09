#ifndef MAPLAYER_H
#define MAPLAYER_H

#include <QString>
#include <QDate>

class MapLayer
{
private:
    QString title, ID, format, tileMatrixSet, _template;
    QDate *minDate, *maxDate, *defaultDate;
    int lowerLat, lowerLon, upperLat, upperLon;

    const bool replace(std::string& str, const std::string& from, const std::string& to) const;
public:
    MapLayer(QString title,
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
             int upperLon);

    MapLayer();

    ~MapLayer();

    const QUrl getTileUrl(int row, int column, int zoom, QDate date) const;

    const QString &getTitle() const;
    void setTitle(const QString &newTitle);
    const QString &getID() const;
    void setID(const QString &newID);
    const QString &getFormat() const;
    void setFormat(const QString &newFormat);
    const QString &getTileMatrixSet() const;
    void setTileMatrixSet(const QString &newTileMatrixSet);
    const QString &getTemplate() const;
    void setTemplate(const QString &newTemplate);
    QDate *getMinDate() const;
    void setMinDate(QDate *newMinDate);
    QDate *getMaxDate() const;
    void setMaxDate(QDate *newMaxDate);
    int getLowerLat() const;
    void setLowerLat(int newLowerLat);
    int getLowerLon() const;
    void setLowerLon(int newLowerLon);
    int getUpperLat() const;
    void setUpperLat(int newUpperLat);
    int getUpperLon() const;
    void setUpperLon(int newUpperLon);

    QDate *getDefaultDate() const;
    void setDefaultDate(QDate *newDefaultDate);
};

#endif // MAPLAYER_H
