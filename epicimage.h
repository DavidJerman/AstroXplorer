#ifndef EPICIMAGE_H
#define EPICIMAGE_H

#include <QPixmap>
#include <QString>

#include "Position3D.h"
#include "Coordinates2D.h"

class EPICImage
{
private:
    QString title, date, caption, version, coord;
    QPixmap* pixmap;
    Coordinates2D centroidCoordinates;
    Position3D dscovrPosition;
    Position3D lunarPosition;
    Position3D sunPosition;
public:
    EPICImage(const QString title,
              const QString date,
              const QString caption,
              const QString version,
              const QString coord,
              const Coordinates2D centroidCoordinates,
              const Position3D dscovrPosition,
              const Position3D lunarPosition,
              const Position3D sunPosition);

    EPICImage();

    ~EPICImage();

    const QString &getTitle() const;

    void setTitle(const QString &newTitle);

    const QString &getDate() const;

    void setDate(const QString &newDate);

    const QString &getCaption() const;

    void setCaption(const QString &newCaption);

    const QString &getVersion() const;

    void setVersion(const QString &newVersion);

    const QString &getCoord() const;

    void setCoord(const QString &newCoord);

    QPixmap *getPixmap() const;

    void setPixmap(QPixmap *newPixmap);

    const Coordinates2D &getCentroidCoordinates() const;

    void setCentroidCoordinates(const Coordinates2D &newCentroidCoordinates);

    const Position3D &getDscovrPosition() const;

    void setDscovrPosition(const Position3D &newDscovrPosition);

    const Position3D &getLunarPosition() const;

    void setLunarPosition(const Position3D &newLunarPosition);

    const Position3D &getSunPosition() const;

    void setSunPosition(const Position3D &newSunPosition);

    const QString centroidToString() const;

    const QString dscovrToString() const;

    const QString lunarToString() const;

    const QString sunToString() const;

    const QString moonToEarth() const;

    const QString sunToEarth() const;

    const QString dscovrToSun() const;

    const QString dscovrToEarth() const;

    static double distance(double x1, double y1, double z1, double x2, double y2, double z2);

    static double distance(const Position3D p1, const Position3D p2);
};

#endif // EPICIMAGE_H
