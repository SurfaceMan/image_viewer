#ifndef TYPES_H
#define TYPES_H

#include <QPainter>

struct PaintInfo {
    QPainter *painter    = nullptr;
    double    worldScale = 1.;
    QSizeF    size;
    QPointF   offset;
};

#endif // TYPES_H
