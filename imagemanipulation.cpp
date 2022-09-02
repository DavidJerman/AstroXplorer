#include "imagemanipulation.h"

ImageManipulation::ImageManipulation() {

}

void ImageManipulation::roundEdges(QPixmap &p, int radius) {
    auto target = QPixmap(p.size());
    target.fill(Qt::transparent);

    QPainter painter(&target);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPainterPath path = QPainterPath();
    path.addRoundedRect(0, 0, p.rect().width(), p.rect().height(), radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, p);

    p = target;
}
