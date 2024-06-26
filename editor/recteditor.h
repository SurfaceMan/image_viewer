#ifndef RECTEDITOR_H
#define RECTEDITOR_H

#include "labeleditor.h"

class RectEditor : public LabelEditor {
public:
    RectEditor();

    void onPaint(const PaintInfo &info) override;

    bool select(const QPointF &pos) override;
    void moving(const QPointF &curPos, const QPointF &lastPos) override;
    void release() override;

    QRectF rect() const;
    void   setRect(const QRectF &value);

    QPen getOutlinePen(const PaintInfo &info) const;

private:
    QRectF mRect;

    enum Handle { TOP_LEFT, BOTTOM_RIGHT } mHandle = BOTTOM_RIGHT;

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

#endif // RECTEDITOR_H
