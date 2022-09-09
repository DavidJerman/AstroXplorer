#include "maptile.h"

MapTile::MapTile(unsigned int row, unsigned int column, QPixmap* pixmap)
    : pixmap(pixmap),
      row(row),
      column(column)
{

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
