#ifndef RINGEDITOR_H
#define RINGEDITOR_H

#include "labeleditor.h"

class RingEditor : public LabelEditor {
public:
    RingEditor();

    void onPaint(const PaintInfo &info) override;

    bool select(const QPointF &pos) override;
    void moving(const QPointF &curPos, const QPointF &lastPos) override;
    void release() override;

    double  insideRadius() const;
    double  outsideRadius() const;
    QPointF center() const;
    void    setRing(const QPointF &center, double insideRadius, double outsideRadius);

private:
    void updateHandle();
    void updateRing(const QPointF &center, double insideRadius, double outsideRadius);
    QPen getOutlinePen(const PaintInfo &info) const;

private:
    QPointF mCenter;
    double  mInsideRadius  = 0;
    double  mOutsideRadius = 0;

    enum Handle { INSIDE = 0, OUTSIDE = 1 } mHandle = OUTSIDE;

    const int handleCount = 2;
    QPointF   mHandles[ 2 ];

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

#endif // RINGEDITOR_H
