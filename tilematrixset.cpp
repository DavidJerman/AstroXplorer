#include "tilematrixset.h"

const QString &TileMatrixSet::getID() const
{
    return ID;
}

void TileMatrixSet::setID(const QString &newID)
{
    ID = newID;
}

unsigned int TileMatrixSet::getMinTileMatrix() const
{
    return minTileMatrix;
}

void TileMatrixSet::setMinTileMatrix(unsigned int newMinTileMatrix)
{
    minTileMatrix = newMinTileMatrix;
}

unsigned int TileMatrixSet::getMaxTileMatrix() const
{
    return maxTileMatrix;
}

void TileMatrixSet::setMaxTileMatrix(unsigned int newMaxTileMatrix)
{
    maxTileMatrix = newMaxTileMatrix;
}

TileMatrixSet::TileMatrixSet(QString ID)
    : ID(std::move(ID))
{

}

TileMatrixSet::~TileMatrixSet() {
    for (auto &matrix: matrices)
        delete matrix;
}

const void TileMatrixSet::addMatrix(TileMatrix* matrix) {
    matrices.push_back(matrix);
}

const unsigned int TileMatrixSet::getW(unsigned int matrix) {
    for (const auto &_matrix: matrices)
        if (matrix == _matrix->ID)
            return _matrix->width;
    return 0;
}

const unsigned int TileMatrixSet::getH(unsigned int matrix) {
    for (const auto &_matrix: matrices)
        if (matrix == _matrix->ID)
            return _matrix->ID;
    return 0;
}
