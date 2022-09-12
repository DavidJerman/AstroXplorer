#ifndef MAPTILE_H
#define MAPTILE_H

#include <QPixmap>
#include <QDate>

class MapTile
{
private:
    QPixmap* pixmap;
    unsigned int row;
    unsigned int column;
    unsigned int zoom;
    QDate* date;
public:
    MapTile();

    ~MapTile();

    MapTile(unsigned int row, unsigned int column, unsigned int zoom, QDate* date, QPixmap* pixmap = nullptr);

    QPixmap *getPixmap() const;
    void setPixmap(QPixmap *newPixmap);
    unsigned int getRow() const;
    void setRow(unsigned int newRow);
    unsigned int getColumn() const;
    void setColumn(unsigned int newColumn);

    unsigned int getZoom() const;
    void setZoom(unsigned int newZoom);


    QDate *getDate() const;
    void setDate(QDate *newDate);
};

#endif // MAPTILE_H
