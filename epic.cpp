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
    if (*++iterator) return *iterator;
    else {
        iterator = images.begin();
        return getCurrentImage();
    }
}

EPICImage* EPIC::getPrevImage() {
    if (iterator == images.begin()) {
        iterator = images.end() - 1;
        return *iterator;
    } else return *--iterator;
}

EPICImage* EPIC::getCurrentImage() {
    auto ret = *iterator;
    return ret;
}

void EPIC::addImage(EPICImage* image) {
    if (!image) return;
    images.push_back(image);
}

void EPIC::reset() {
    iterator = images.begin();
    sort();
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
