#ifndef EPIC_H
#define EPIC_H

#include <vector>
#include <QPixmap>
#include "epicimage.h"

class EPIC
{
private:
    static std::vector<EPICImage*> images;
    static std::vector<EPICImage*>::iterator iterator;
public:
    EPIC();

    static void clear();

    static EPICImage* getNextImage();

    static EPICImage* getPrevImage();

    static EPICImage* getCurrentImage();

    static void addImage(EPICImage* image);

    static void reset();

    static void sort();
};

#endif // EPIC_H
