#ifndef UTILS_H
#define UTILS_H

#include <QLineF>
#include <QPointF>

double distance(const QPointF &p1, const QPointF &p2);

double distance2(const QPointF &p1, const QPointF &p2);

double distance2_sq(const QPointF &p1, const QPointF &p2);

double distance(const QPointF &point, const QLineF &line);

#endif
