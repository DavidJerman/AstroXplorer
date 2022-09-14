#ifndef TILEMATRIXSET_H
#define TILEMATRIXSET_H

#include <vector>
#include <QString>
#include "TileMatrix.h"

class TileMatrixSet
{
private:
    QString ID;
    unsigned int minTileMatrix {0};
    unsigned int maxTileMatrix {0};
    std::vector<TileMatrix*> matrices;
public:
    TileMatrixSet(QString ID);

    ~TileMatrixSet();

    const void addMatrix(TileMatrix* matrix);

    const unsigned int getW(unsigned int matrix);

    const unsigned int getH(unsigned int matrix);

    const QString &getID() const;
    void setID(const QString &newID);
    unsigned int getMinTileMatrix() const;
    void setMinTileMatrix(unsigned int newMinTileMatrix);
    unsigned int getMaxTileMatrix() const;
    void setMaxTileMatrix(unsigned int newMaxTileMatrix);
};

#endif // TILEMATRIXSET_H
