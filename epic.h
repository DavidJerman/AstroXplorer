#ifndef EPIC_H
#define EPIC_H

#include <vector>
#include <QPixmap>
#include "epicimage.h"

class EPIC
{
private:
    static std::vector<EPICImage*> images;
    static std::vector<QPixmap*>::iterator iterator;
public:
    EPIC();

    void clear();

    QPixmap* getNextPixmap() const;

    QPixmap* getPrevPixmap() const;

    QPixmap* getCurrentPixmap() const;

    void addPixmap(QPixmap* pixmap);

    void reset();
};

#endif // EPIC_H
