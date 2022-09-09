#ifndef MAPTILE_H
#define MAPTILE_H

#include <QPixmap>

class MapTile
{
private:
    QPixmap* pixmap;
    unsigned int row;
    unsigned int column;
public:
    MapTile();

    ~MapTile();

    MapTile(unsigned int row, unsigned int column, QPixmap* pixmap = nullptr);

    QPixmap *getPixmap() const;
    void setPixmap(QPixmap *newPixmap);
    unsigned int getRow() const;
    void setRow(unsigned int newRow);
    unsigned int getColumn() const;
    void setColumn(unsigned int newColumn);
};

#endif // MAPTILE_H
