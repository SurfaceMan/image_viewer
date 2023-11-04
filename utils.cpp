#include "utils.h"

double distance(const QPointF &p1, const QPointF &p2) {
    auto delta = p1 - p2;
    // return sqrt(delta.x() * delta.x() + delta.y() * delta.y());
    return delta.manhattanLength();
}

double distance2(const QPointF &p1, const QPointF &p2) {
    auto delta = p1 - p2;
    return sqrt(delta.x() * delta.x() + delta.y() * delta.y());
}

double distance2_sq(const QPointF &p1, const QPointF &p2) {
    auto delta = p1 - p2;
    return delta.x() * delta.x() + delta.y() * delta.y();
}
