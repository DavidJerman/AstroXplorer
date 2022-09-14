#ifndef TILEMATRIX_H
#define TILEMATRIX_H

#endif // TILEMATRIX_H

struct TileMatrix {
    TileMatrix(unsigned int width, unsigned int height, unsigned int ID)
        : width(width), height(height), ID(ID) {};
    unsigned int ID;
    unsigned int width;
    unsigned int height;
};
