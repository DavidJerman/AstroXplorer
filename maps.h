#ifndef MAPS_H
#define MAPS_H

#include <vector>
#include <string>
#include <QString>
#include <QDate>
#include "maptile.h"
#include "maplayer.h"

class Maps
{
private:
    static std::vector<MapLayer*> availableLayers;
    std::vector<MapTile*> tiles;
    std::vector<MapTile*> tilesInQueue;
    std::vector<MapTile*> requestedTiles;
    MapLayer* activeLayer;
    QDate activeDate;
    float lat, lon;
    int zoom {2};
    int minZoom {0}, maxZoom {2};
    int maxColumn {5}, maxRow {2};

    static void sort();
public:
    Maps(MapLayer* layer, float lat = 0, float lon = 0);

    ~Maps();

    static void clearLayers();

    static void addLayersFromXML(QString fileName);

    static void addLayersFromXML(std::string fileName);

    static const std::vector<MapLayer*>& getLayers();

    static bool hasLayers();

    static float latToY(float lat);

    static float lonToX(float lon);

    static float XToLon(float X);

    static float YToLat(float Y);

    void update(float latA, float lonA, float latB, float lonB);

    static bool containsTile(unsigned int row, unsigned int column, const std::vector<MapTile*>& tiles);

    template <typename T>
    const T max(const T t1, const T t2);

    template <typename T>
    const T min(const T t1, const T t2);

    // Tile retrivial and adding
    bool isTileQueueEmpty() const;

    MapTile* popQueuedTile();

    bool isRequestQueueEmpty() const;

    void addTileToRequested(MapTile* tile);

    MapTile* popRequestedTile();

    MapTile* findInRequestedTiles(unsigned int row, unsigned int column, unsigned int zoom, const QDate& date);

    bool isTilesEmpty() const;

    void addActiveTile(MapTile* tile);

    void clear();

    const QString getID() const;

    QUrl getTileUrl(MapTile*) const;

    // Getters

    MapLayer *getActiveLayer() const;
    const QDate &getActiveDate() const;
    float getLat() const;
    float getLon() const;
    int getZoom() const;

    void setLat(float newLat);
    void setLon(float newLon);
    void setZoom(int newZoom);
    int getMinZoom() const;
    void setMinZoom(int newMinZoom);
    int getMaxZoom() const;
    void setMaxZoom(int newMaxZoom);
    int getMaxColumn() const;
    void setMaxColumn(int newMaxColumn);
    int getMaxRow() const;
    void setMaxRow(int newMaxRow);


    const std::vector<MapTile *> &getTiles() const;
};

#endif // MAPS_H
