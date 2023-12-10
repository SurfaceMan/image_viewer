#ifndef LABELPOINT_H
#define LABELPOINT_H

#include "label.h"

class LabelPoint : public Label {
public:
    void setPoints(QPolygonF points);
    void setPoints(QPolygonF points, QPolygonF vector);

    void onPaint(const PaintInfo &info) override;

private:
    QPolygonF mPoints;
    QPolygonF mVectors;

    QVector<QPointF> mLines;
    double           mLastScale = -1;

    const float mVectorLength    = 20;
    const float mPointSizeRadius = 5;
    const float mLineWidth       = 3;
};

#endif // LABELPOINT_H
