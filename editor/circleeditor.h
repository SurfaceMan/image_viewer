#ifndef CIRCLEEDITOR_H
#define CIRCLEEDITOR_H

#include "labeleditor.h"

class CircleEditor : public LabelEditor {
public:
    CircleEditor();

    void onPaint(const PaintInfo &info) override;

    bool select(const QPointF &pos) override;
    void moving(const QPointF &curPos, const QPointF &lastPos) override;
    void release() override;

    double  radius() const;
    QPointF center() const;
    void    setCircle(const QPointF &center, double radius);

private:
    void updateHandle();
    void updateCircle(const QPointF &center, double radius);
    QPen getOutlinePen(const PaintInfo &info) const;

private:
    QPointF mCenter;
    double  mRadius = 0;

    enum Handle { TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3 } mHandle = TOP;

    const int handleCount = 4;
    QPointF   mHandles[ 4 ];

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

#endif // CIRCLEEDITOR_H
