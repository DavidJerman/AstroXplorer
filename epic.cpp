#include "epic.h"
#include <QDate>

std::vector<EPICImage*> EPIC::images;
std::vector<EPICImage*>::iterator EPIC::iterator;
std::vector<QDate*> EPIC::availableDates;

EPIC::EPIC()
{

}

void EPIC::clear() {
    for (auto &e: images) delete e;
    images.clear();
    reset();
}

EPICImage* EPIC::getNextImage() {
    if (images.empty()) return {};
    if (iterator == images.end() - 1) {
        iterator = images.begin();
        return *iterator;
    } else return *++iterator;
}

EPICImage* EPIC::getPrevImage() {
    if (images.empty()) return {};
    if (iterator == images.begin()) {
        iterator = images.end() - 1;
        return *iterator;
    } else return *--iterator;
}

EPICImage* EPIC::getCurrentImage() {
    if (images.empty()) return {};
    auto ret = *iterator;
    return ret;
}

void EPIC::addImage(EPICImage* image) {
    if (!image) return;
    images.push_back(image);
}

void EPIC::reset() {
    sort();
    iterator = images.begin();
}

void EPIC::sort() {
    std::sort(images.begin(), images.end(), [](const EPICImage* i1, const EPICImage* i2) {
        return i1->getTitle() < i2->getTitle();
    });
}

void EPIC::clearDates() {
    for (auto date: availableDates) delete date;
    availableDates.clear();
}

void EPIC::sortDates() {
    std::sort(availableDates.begin(), availableDates.end(), [](const QDate* d1, const QDate *d2) {
        return (*d1) < (*d2);
    });
}

void EPIC::addDate(QDate* date) {
    if (!date) return;
    availableDates.push_back(date);
}

QDate* EPIC::getMaxDateWhenSorted() {
    return availableDates[availableDates.size() - 1];
}

QDate* EPIC::getMinDateWhenSorted() {
    return availableDates[0];
}

qint64 EPIC::getTotalDates() {
    return availableDates.size();
}

QDate* EPIC::getDate(qint64 index) {
    if (index < 0 || index >= availableDates.size()) return availableDates[0];
    return availableDates[index];
}

double EPIC::distance(double x1, double y1, double z1, double x2, double y2, double z2) {
    return (std::sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2)));
}

double EPIC::distance(Position3D& p1, Position3D& p2) {
    return distance(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}
