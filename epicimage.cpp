#include "epicimage.h"

const Coordinates2D &EPICImage::getCentroidCoordinates() const
{
    return centroidCoordinates;
}

void EPICImage::setCentroidCoordinates(const Coordinates2D &newCentroidCoordinates)
{
    centroidCoordinates = newCentroidCoordinates;
}

const Position3D &EPICImage::getDscovrPosition() const
{
    return dscovrPosition;
}

void EPICImage::setDscovrPosition(const Position3D &newDscovrPosition)
{
    dscovrPosition = newDscovrPosition;
}

const Position3D &EPICImage::getLunarPosition() const
{
    return lunarPosition;
}

void EPICImage::setLunarPosition(const Position3D &newLunarPosition)
{
    lunarPosition = newLunarPosition;
}

const Position3D &EPICImage::getSunPosition() const
{
    return sunPosition;
}

void EPICImage::setSunPosition(const Position3D &newSunPosition)
{
    sunPosition = newSunPosition;
}

EPICImage::EPICImage(const QString title,
                     const QString date,
                     const QString caption,
                     const QString version,
                     const QString coord,
                     const Coordinates2D centroidCoordinates,
                     const Position3D dscovrPosition,
                     const Position3D lunarPosition,
                     const Position3D sunPosition)
    : title(std::move(title)),
      date(std::move(date)),
      caption(std::move(caption)),
      version(std::move(version)),
      coord(std::move(coord)),
      centroidCoordinates(std::move(centroidCoordinates)),
      dscovrPosition(std::move(dscovrPosition)),
      lunarPosition(std::move(lunarPosition)),
      sunPosition(std::move(sunPosition))
{

}

EPICImage::~EPICImage() {
    delete pixmap;
}

const QString &EPICImage::getTitle() const
{
    return title;
}

void EPICImage::setTitle(const QString &newTitle)
{
    title = newTitle;
}

const QString &EPICImage::getDate() const
{
    return date;
}

void EPICImage::setDate(const QString &newDate)
{
    date = newDate;
}

const QString &EPICImage::getCaption() const
{
    return caption;
}

void EPICImage::setCaption(const QString &newCaption)
{
    caption = newCaption;
}

const QString &EPICImage::getVersion() const
{
    return version;
}

void EPICImage::setVersion(const QString &newVersion)
{
    version = newVersion;
}

const QString &EPICImage::getCoord() const
{
    return coord;
}

void EPICImage::setCoord(const QString &newCoord)
{
    coord = newCoord;
}

QPixmap *EPICImage::getPixmap() const
{
    return pixmap;
}

void EPICImage::setPixmap(QPixmap *newPixmap)
{
    pixmap = newPixmap;
}

const QString EPICImage::centroidToString() const {
    return "Lat: " + QString::number(centroidCoordinates.lat) + " Lon: " + QString::number(centroidCoordinates.lon);
}

const QString EPICImage::dscovrToString() const {
    return "X: " + QString::number(dscovrPosition.x) + " Y: " + QString::number(dscovrPosition.y) + " Z: " + QString::number(dscovrPosition.z);
}

const QString EPICImage::lunarToString() const {
    return "X: " + QString::number(lunarPosition.x) + " Y: " + QString::number(lunarPosition.y) + " Z: " + QString::number(lunarPosition.z);
}

const QString EPICImage::sunToString() const {
    return "X: " + QString::number(sunPosition.x) + " Y: " + QString::number(sunPosition.y) + " Z: " + QString::number(sunPosition.z);
}

const QString EPICImage::moonToEarth() const {
    return QString::number((int)distance(lunarPosition, {0, 0, 0}), 'g', 10) + " km";
}

const QString EPICImage::sunToEarth() const {
    return QString::number((int)distance(sunPosition, {0, 0, 0}), 'g', 10) + " km";
}

const QString EPICImage::dscovrToSun() const {
    return QString::number((int)distance(dscovrPosition, sunPosition), 'g', 10) + " km";
}

const QString EPICImage::dscovrToEarth() const {
    return QString::number((int)distance(dscovrPosition, {0, 0, 0}), 'g', 10) + " km";
}

double EPICImage::distance(double x1, double y1, double z1, double x2, double y2, double z2) {
    return (std::sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2)));
}

double EPICImage::distance(const Position3D p1, const Position3D p2) {
    return distance(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}

