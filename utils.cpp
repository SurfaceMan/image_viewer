#include "utils.h"

#include <cmath>

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

double distance(const QPointF &point, const QLineF &line) {
    auto x1 = line.x1();
    auto y1 = line.y1();
    auto x2 = line.x2();
    auto y2 = line.y2();

    auto u = ((point.x() - x1) * (x2 - x1) + (point.y() - y1) * (y2 - y1)) /
             (line.length() * line.length());
    auto x = x1 + u * (x2 - x1);
    auto y = y1 + u * (y2 - y1);

    return QLineF(point, QPointF(x, y)).length();
}
