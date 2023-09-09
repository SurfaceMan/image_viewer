#ifndef TYPES_H
#define TYPES_H

#include <QPainter>

struct PaintInfo {
    QPainter *painter    = nullptr;
    double    worldScale = 1.;
    // bool      isSelected    = false;
    // bool      isHighlighted = false;
};

struct WorldInfo {
    QPointF position;
    float   angle      = 0;
    float   worldScale = 1.f;
};

#endif // TYPES_H
