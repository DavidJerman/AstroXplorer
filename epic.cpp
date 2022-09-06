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
