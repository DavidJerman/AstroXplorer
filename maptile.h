#ifndef MAPTILE_H
#define MAPTILE_H

#include <QPixmap>

class MapTile
{
private:
    QPixmap* pixmap;
    unsigned int x;
    unsigned int y;
    float zoom;
public:
    MapTile();

    ~MapTile();

    MapTile(QPixmap* pixmap, unsigned int x, unsigned int y);
};

#endif // MAPTILE_H
