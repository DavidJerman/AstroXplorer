#include "maptile.h"

MapTile::MapTile(unsigned int row, unsigned int column, unsigned int zoom, QDate* date, QPixmap* pixmap)
    : pixmap(pixmap),
      row(row),
      column(column),
      zoom(zoom),
      date(date)
{

}

unsigned int MapTile::getZoom() const
{
    return zoom;
}

void MapTile::setZoom(unsigned int newZoom)
{
    zoom = newZoom;
}

QDate *MapTile::getDate() const
{
    return date;
}

void MapTile::setDate(QDate *newDate)
{
    date = newDate;
}

MapTile::MapTile()
    : pixmap(nullptr),
      row(0),
      column(0)
{

}

MapTile::~MapTile() {
    delete pixmap;
}

QPixmap *MapTile::getPixmap() const
{
    return pixmap;
}

void MapTile::setPixmap(QPixmap *newPixmap)
{
    delete pixmap;
    pixmap = newPixmap;
}

unsigned int MapTile::getRow() const
{
    return row;
}

void MapTile::setRow(unsigned int newRow)
{
    row = newRow;
}

unsigned int MapTile::getColumn() const
{
    return column;
}

void MapTile::setColumn(unsigned int newColumn)
{
    column = newColumn;
}
