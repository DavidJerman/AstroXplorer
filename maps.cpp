#include "maps.h"

#include <QFile>
#include <QDomDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

std::vector<MapLayer*> Maps::availableLayers {};
std::vector<TileMatrixSet*> Maps::tileMatrixSets {};

// Date is temporary, will remove
Maps::Maps(MapLayer* layer, float lat, float lon)
    : activeLayer(new MapLayer(*layer)), lat(lat), lon(lon), activeDate(2020, 06, 11)
{

}

Maps::~Maps()
{

}

void Maps::clearLayers() {
    for (auto &layer: availableLayers) delete layer;
    availableLayers.clear();
}

void Maps::addLayersFromXML(QString fileName) {
    QFile file;
    file.setFileName(std::move(fileName));
    if (file.open(QIODevice::ReadOnly)) {
        auto dom = new QDomDocument();
        dom->setContent(&file);
        auto root = dom->documentElement();
        qDebug() << root.tagName();
        auto item = root.firstChild().toElement();
        while (!item.isNull()) {
            if (item.tagName() == "Contents") {
                auto layerEl = item.firstChild().toElement();
                while (!layerEl.isNull()) {
                    auto layer = new MapLayer();
                    auto layerItem = layerEl.firstChild().toElement();
                    // Temporary variables
                    QString title, ID, format, tileMatrixSet, _template;
                    QDate *minDate, *maxDate;
                    int lowerLat, lowerLon, upperLat, upperLon;
                    while (!layerItem.isNull()) {
                        // Set values
                        auto tag = layerItem.tagName();
                        if (tag == "ows:Title") layer->setTitle(layerItem.text());
                        else if (tag == "ows:WGS84BoundingBox") {
                            auto lower = layerItem.firstChild().toElement().text().toStdString();
                            auto upper = layerItem.firstChild().nextSibling().toElement().text().toStdString();
                            layer->setLowerLon(stoi(lower.substr(0, lower.find(" "))));
                            layer->setLowerLat(stoi(lower.substr(lower.find(" ") + 1)));
                            layer->setUpperLon(stoi(upper.substr(0, upper.find(" "))));
                            layer->setUpperLat(stoi(upper.substr(upper.find(" ") + 1)));
                        }
                        else if (tag == "ows:Identifier") layer->setID(layerItem.text());
                        else if (tag == "Format") layer->setFormat(layerItem.text());
                        else if (tag == "Dimension") {
                            auto sub = layerItem.firstChild().toElement();
                            while (!sub.isNull()) {
                                sub = sub.nextSibling().toElement();
                                tag = sub.tagName();
                                if (tag == "Default") layer->setDefaultDate(new QDate(QDate::fromString(sub.text(), "yyyy-MM-dd")));
                            }
                        }
                        else if (tag == "TileMatrixSetLink") layer->setTileMatrixSet(layerItem.firstChild().toElement().text());
                        else if (tag == "ResourceURL") {
                            if (layerItem.hasAttribute("format"))
                                layer->setFormat(layerItem.attribute("format"));
                            if (layerItem.hasAttribute("template"))
                                layer->setTemplate(layerItem.attribute("template"));
                        }
                        layerItem = layerItem.nextSibling().toElement();
                    }
                    layerEl = layerEl.nextSibling().toElement();
                    availableLayers.push_back(layer);
                }
            }
            item = item.nextSibling().toElement();
        }
    }
    file.close();
    sort();
}

void Maps::addLayersFromXML(std::string fileName) {
    addLayersFromXML(QString::fromStdString(std::move(fileName)));
}


const std::vector<MapLayer*>& Maps::getLayers() {
    return availableLayers;
}

bool Maps::hasLayers() {
    return availableLayers.empty();
}

MapLayer *Maps::getActiveLayer() const
{
    return activeLayer;
}

const QDate &Maps::getActiveDate() const
{
    return activeDate;
}

float Maps::getLat() const
{
    return lat;
}

float Maps::getLon() const
{
    return lon;
}

int Maps::getZoom() const
{
    return zoom;
}

void Maps::setLat(float newLat)
{
    lat = newLat;
}

void Maps::setLon(float newLon)
{
    lon = newLon;
}

void Maps::setZoom(int newZoom)
{
    zoom = newZoom;
}

int Maps::getMinZoom() const
{
    return minZoom;
}

void Maps::setMinZoom(int newMinZoom)
{
    minZoom = newMinZoom;
}

int Maps::getMaxZoom() const
{
    return maxZoom;
}

void Maps::setMaxZoom(int newMaxZoom)
{
    maxZoom = newMaxZoom;
}

int Maps::getMaxColumn() const
{
    return maxColumn;
}

void Maps::setMaxColumn(int newMaxColumn)
{
    maxColumn = newMaxColumn;
}

int Maps::getMaxRow() const
{
    return maxRow;
}

void Maps::setMaxRow(int newMaxRow)
{
    maxRow = newMaxRow;
}

const std::vector<MapTile *> &Maps::getTiles() const
{
    return tiles;
}

void Maps::sort() {
    std::sort(availableLayers.begin(), availableLayers.end(), [](const MapLayer* const l1, const MapLayer* const l2){
        return l1->getTitle() < l2->getTitle();
    });
}

float Maps::latToY(float lat) {
    return lat + 90;
}

float Maps::lonToX(float lon) {
    return lon + 180;
}

float Maps::XToLon(float X) {
    return X - 180;
}

float Maps::YToLat(float Y) {
    return Y - 90;
}

void Maps::update(float latA, float lonA, float latB, float lonB, QTableWidget *table) {
    // This will request the right map pieces and remove reduntant ones
    if (!activeLayer) return;
    auto midLat = (latA + latB) / 2;
    auto midLon = (lonA + lonB) / 2;
    auto diffLat = std::abs(midLat - latA);
    auto diffLon = std::abs(midLon - lonA);
    auto maxLon = std::min((float)360, lonToX(activeLayer->getUpperLon()) - lonToX(activeLayer->getLowerLon()));
    auto maxLat = std::min((float)180, latToY(activeLayer->getUpperLat()) - latToY(activeLayer->getLowerLat()));
    auto minLon = std::max((float)0, lonToX(activeLayer->getLowerLon()));
    auto minLat = std::max((float)0, latToY(activeLayer->getLowerLat()));
    auto xA = (int)(max(lonToX(lonA) - lonToX(diffLon) / 2, minLon)
                    / (maxLon / (maxColumn)));
    auto yA = (int)(max(latToY(latA) - latToY(diffLat) / 2, minLat)
                    / (maxLat / (maxRow)));
    auto xB = (int)(min(lonToX(lonB) + lonToX(diffLon) / 2, maxLon)
                    / (maxLon / (maxColumn)));
    auto yB = (int)(min(latToY(latB) + latToY(diffLat) / 2, maxLat)
                    / (maxLat / (maxRow)));
    // Puts new tiles in queue for download
    for (int x = xA; x <= xB; x++) {
        for (int y = yA; y <= yB; y++) {
            if (!containsTile(y, x, tiles) && !containsTile(y, x, tilesInQueue) && !containsTile(y, x, requestedTiles))
                tilesInQueue.push_back(new MapTile(y, x, zoom, new QDate(activeDate)));
        }
    }
    // Deletes/removes old unused tiles
    for (int x = 0; x <= maxColumn; x++) {
        for (int y = 0; y <= maxRow; y++) {
            if (x >= xA && x <= xB && y >= yA && y <= yB) continue;
            auto item = std::find_if(tiles.begin(), tiles.end(), [&x, &y](const MapTile* const tile) -> bool {
                if (!tile) return true;
                return tile->getColumn() == x && tile->getRow() == y;
            });
            if (item != tiles.end()) {
                delete *item;
                tiles.erase(item);
                table->removeCellWidget(y, x);
                // delete *item;
            }
        }
    }
}

template <typename T>
const T Maps::max(const T t1, const T t2) {
    if (t1 > t2) return t1;
    else return t2;
}

template <typename T>
const T Maps::min(const T t1, const T t2) {
    if (t1 < t2) return t1;
    else return t2;
}

bool Maps::containsTile(unsigned int row, unsigned int column, const std::vector<MapTile*>& tiles) {
    for (const auto &tile: tiles)
        if (tile->getRow() == row && tile->getColumn() == column)
            return true;
    return false;
}

// Tile retrivial and adding
bool Maps::isTileQueueEmpty() const {
    return tilesInQueue.empty();
}

MapTile* Maps::popQueuedTile() {
    if (tilesInQueue.empty()) return {};
    auto temp = tilesInQueue.back();
    tilesInQueue.pop_back();
    return temp;
}


bool Maps::isRequestQueueEmpty() const {
    return requestedTiles.empty();
}

void Maps::addTileToRequested(MapTile* tile) {
    if (!tile) return;
    requestedTiles.push_back(tile);
}


MapTile* Maps::popRequestedTile() {
    if (requestedTiles.empty()) return {};
    auto temp = requestedTiles.back();
    requestedTiles.pop_back();
    return temp;
}

MapTile* Maps::findInRequestedTiles(unsigned int row, unsigned int column, unsigned int zoom, const QDate& date) {
    if (requestedTiles.empty()) return {};
    for (auto beg = requestedTiles.begin(); beg <= requestedTiles.end(); beg++) {
        if ((*beg)->getColumn() == column && (*beg)->getRow() == row
                && (*beg)->getZoom() == zoom && *(*beg)->getDate() == date) {
            auto temp = (*beg);
            requestedTiles.erase(beg);
            return temp;
        }
    }
    return {};
}

bool Maps::isTilesEmpty() const {
    return tiles.empty();
}

void Maps::addActiveTile(MapTile* tile) {
    if (tile->getPixmap()->isNull()) {
        delete tile;
        return;
    } else {
        tiles.push_back(tile);
    }
}

QUrl Maps::getTileUrl(MapTile* tile) const {
    if (!activeLayer) return {};
    return activeLayer->getTileUrl(tile->getRow(), tile->getColumn(), tile->getZoom(), *tile->getDate());
}

void Maps::clear() {

}


const QString Maps::getID() const {
    if (!activeLayer) return {"Invalid!"};
    else return activeLayer->getID();
}

void Maps::addTileMatrixSet(TileMatrixSet* set) {
    tileMatrixSets.push_back(set);
}


void Maps::addMatrixSetsFromXML(QString fileName) {
    QFile file;
    file.setFileName(std::move(fileName));
    if (file.open(QIODevice::ReadOnly)) {
        auto dom = new QDomDocument();
        dom->setContent(&file);
        auto root = dom->documentElement();
        qDebug() << root.tagName();
        auto item = root.firstChild().toElement();
        while (!item.isNull()) {
            if (item.tagName() == "Contents") {
                auto layerEl = item.firstChild().toElement();
                while (!layerEl.isNull()) {
                    if (layerEl.tagName() == "TileMatrixSet") {
                        auto layerItem = layerEl.firstChild().toElement();
                        QString setID;
                        unsigned int ID, w, h;
                        TileMatrixSet* set {nullptr};
                        while (!layerItem.isNull()) {
                            layerItem = layerItem.nextSibling().toElement();
                            auto tag = layerItem.tagName();
                            if (tag == "ows:Identifier") {
                                setID = layerItem.text();
                                set = new TileMatrixSet(setID);
                            }
                            if (set && tag == "TileMatrix") {
                                auto subItem = layerItem.firstChild().toElement();
                                while (!subItem.isNull()) {
                                    tag = subItem.tagName();
                                    if (tag == "ows:Identifier") ID = subItem.text().toInt();
                                    if (tag == "MatrixWidth") w = subItem.text().toInt();
                                    if (tag == "MatrixHeight") h = subItem.text().toInt();
                                    subItem = subItem.nextSibling().toElement();
                                }
                                set->addMatrix(new TileMatrix(w, h, ID));
                            }
                        }
                        if (set)
                            tileMatrixSets.push_back(set);
                    }
                    layerEl = layerEl.nextSibling().toElement();
                }
            }
            item = item.nextSibling().toElement();
        }
    }
    file.close();
}

void Maps::addMatrixSetsFromXML(std::string fileName) {
    addMatrixSetsFromXML(QString::fromStdString(fileName));
}

