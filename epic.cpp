#include "epic.h"

std::vector<EPICImage*> EPIC::images;
std::vector<EPICImage*>::iterator EPIC::iterator;

EPIC::EPIC()
{

}

void EPIC::clear() {
    for (auto &e: images) delete e;
    images.clear();
    reset();
}

QPixmap* EPIC::getNextPixmap() {
    if (*++iterator) return (*iterator)->getPixmap();
    else return {};
}

QPixmap* EPIC::getPrevPixmap() {
    if (*--iterator) return (*iterator)->getPixmap();
    else return {};
}

QPixmap* EPIC::getCurrentPixmap() {
    return (*iterator)->getPixmap();
}

void EPIC::addImage(EPICImage* image) {
    images.push_back(image);
}

void EPIC::reset() {
    iterator = images.begin();
}
