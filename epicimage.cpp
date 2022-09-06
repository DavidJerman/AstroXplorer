#include "epicimage.h"

const QString &EPICImage::getTitle() const
{
    return title;
}

void EPICImage::setTitle(const QString &newTitle)
{
    title = newTitle;
}

const QString &EPICImage::getDate() const
{
    return date;
}

void EPICImage::setDate(const QString &newDate)
{
    date = newDate;
}

const QString &EPICImage::getCaption() const
{
    return caption;
}

void EPICImage::setCaption(const QString &newCaption)
{
    caption = newCaption;
}

const QString &EPICImage::getVersion() const
{
    return version;
}

void EPICImage::setVersion(const QString &newVersion)
{
    version = newVersion;
}

const QString &EPICImage::getCoord() const
{
    return coord;
}

void EPICImage::setCoord(const QString &newCoord)
{
    coord = newCoord;
}

QPixmap *EPICImage::getPixmap() const
{
    return pixmap;
}

void EPICImage::setPixmap(QPixmap *newPixmap)
{
    pixmap = newPixmap;
}

EPICImage::EPICImage()
{

}
