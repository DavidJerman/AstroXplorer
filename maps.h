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
    std::vector<MapTile*> requestedTiles;
    QDate activeDate;
    float lat, lon;
    int zoom;
    int minZoom, maxZoom;

    static void sort();
public:
    Maps();

    ~Maps();

    static void clearLayers();

    static void addLayersFromXML(QString fileName);

    static void addLayersFromXML(std::string fileName);

    static const std::vector<MapLayer*>& getLayers();

    static bool hasLayers();
};

#endif // MAPS_H
