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

    static QPixmap* getNextPixmap();

    static QPixmap* getPrevPixmap();

    static QPixmap* getCurrentPixmap();

    static void addImage(EPICImage* image);

    static void reset();
};

#endif // EPIC_H
