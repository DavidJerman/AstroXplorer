#include "maps.h"

#include <QFile>
#include <QDomDocument>

std::vector<MapLayer*> Maps::availableLayers {};

Maps::Maps()
{

}

Maps::~Maps()
{

}

void Maps::clearLayers() {
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
    auto temp = availableLayers;
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

void Maps::sort() {
    std::sort(availableLayers.begin(), availableLayers.end(), [](const MapLayer* const l1, const MapLayer* const l2){
        return l1->getTitle() < l2->getTitle();
    });
}
