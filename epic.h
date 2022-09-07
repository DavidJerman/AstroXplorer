#ifndef EPIC_H
#define EPIC_H

#include <vector>
#include <QPixmap>
#include "epicimage.h"
#include <cmath>
#include "Position3D.h"

class EPIC
{
private:
    static std::vector<EPICImage*> images;
    static std::vector<EPICImage*>::iterator iterator;
    static std::vector<QDate*> availableDates;
public:
    EPIC();

    static void clear();

    static const EPICImage* const getNextImage();

    static const EPICImage* const getPrevImage();

    static const EPICImage* const getCurrentImage();

    static void addImage(EPICImage* image);

    static void reset();

    static void sort();

    static void clearDates();

    static void sortDates();

    static void addDate(QDate* date);

    static const QDate* const getMaxDateWhenSorted();

    static const QDate* const getMinDateWhenSorted();

    static qint64 getTotalDates();

    static const QDate* const getDate(qint64 index);

    static double distance(double x1, double y1, double z1, double x2, double y2, double z2);

    static double distance(Position3D& p1, Position3D& p2);
};

#endif // EPIC_H
