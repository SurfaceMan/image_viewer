#ifndef ROTATEDRECTLABEL_H
#define ROTATEDRECTLABEL_H

#include "labeleditor.h"

#include <QPolygonF>

class RotatedRectLabel : public Label {
public:
    RotatedRectLabel();

    void onPaint(const PaintInfo &info) override;

    double angle() const;
    QRectF rect() const;
    void   setRotatedRect(const QRectF &rect, double angle);

private:
    void updatePoint();
    void updateRotatedRect(const QRectF &rect, double angle);
    QPen getOutlinePen(const PaintInfo &info) const;

private:
    QRectF mRect;
    double mAngle = 0;

    QPolygonF mRectPoints;
};

#endif // ROTATEDRECTLABEL_H
