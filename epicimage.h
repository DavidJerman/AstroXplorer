#ifndef EPICIMAGE_H
#define EPICIMAGE_H

#include <QPixmap>
#include <QString>

class EPICImage
{
private:
    QString title, date, caption, version, coord;
    QPixmap* pixmap;
public:
    EPICImage(const QString title, const QString date, const QString caption, const QString version, const QString coord);

    EPICImage();

    const QString &getTitle() const;

    void setTitle(const QString &newTitle);

    const QString &getDate() const;

    void setDate(const QString &newDate);

    const QString &getCaption() const;

    void setCaption(const QString &newCaption);

    const QString &getVersion() const;

    void setVersion(const QString &newVersion);

    const QString &getCoord() const;

    void setCoord(const QString &newCoord);

    QPixmap *getPixmap() const;

    void setPixmap(QPixmap *newPixmap);
};

#endif // EPICIMAGE_H
