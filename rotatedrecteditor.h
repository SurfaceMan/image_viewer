#ifndef ROTATEDRECTEDITOR_H
#define ROTATEDRECTEDITOR_H

#include "labeleditor.h"

#include <QPolygonF>

class RotatedRectEditor : public LabelEditor {
public:
    RotatedRectEditor();

    void onPaint(const PaintInfo &info) override;
    bool select(const QPointF &pos) override;
    void moving(const QPointF &curPos, const QPointF &lastPos) override;
    void release() override;
    void rotate(double angleDelta) override;

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
    QPolygonF mHandlePoints;
    const int handleCount = 4;

    enum Handle { TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3 } mHandle = TOP;

    const double mHandleDistanceBase = 3.;
    double       mHandleDistance     = 3.;
    const double mCrossLengthBase    = 10.;
    double       mCrossLength        = 10.0;

    bool mPressed           = false;
    bool mSelected          = false;
    bool mHighLighted       = false;
    bool mHandleSelected    = false;
    bool mHandleHighLighted = false;
};

#endif // ROTATEDRECTEDITOR_H
