#include "epic.h"

std::vector<EPICImage*> EPIC::images;
std::vector<QPixmap*>::iterator EPIC::iterator;

EPIC::EPIC()
{

}

void EPIC::clear() {
    for (auto &e: images) delete e;
    images.clear();
    reset();
}

QPixmap* EPIC::getNextPixmap() const {
    if (*++iterator) return *iterator;
    else return {};
}

QPixmap* EPIC::getPrevPixmap() const {
    if (*--iterator) return *iterator;
    else return {};
}

QPixmap* EPIC::getCurrentPixmap() const {
    return *iterator;
}

void EPIC::addPixmap(QPixmap* pixmap) {
    // images.push_back(pixmap);
}

void EPIC::reset() {
    // iterator = images.begin();
}
