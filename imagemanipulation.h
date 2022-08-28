#ifndef IMAGEMANIPULATION_H
#define IMAGEMANIPULATION_H

#include <QPixmap>
#include <QPainter>
#include <QPainterPath>

class ImageManipulation
{
public:
    ImageManipulation();

    static void roundEdges(QPixmap &p, int radius);
};

#endif // IMAGEMANIPULATION_H
