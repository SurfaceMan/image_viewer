#ifndef LABELRECT_H
#define LABELRECT_H

#include "labeleditor.h"

class RectEditor : public LabelEditor {
public:
    RectEditor();

    void        onPaint(const PaintInfo &info) override;
    QStringList serialize() const override;
    void        deserialize(const QStringList &strs) override;

    bool select(const QPointF &pos) override;
    void moving(const QPointF &curPos, const QPointF &lastPos) override;
    void release() override;

    QPen getOutlinePen(const PaintInfo &info) const;

private:
    QRectF mRect;

    enum Handle { TOP_LEFT, BOTTOM_RIGHT } mHandle = BOTTOM_RIGHT;

    const double mHandleDistanceBase = 3.;
    double       mHandleDistance     = 3.;

    bool mPressed           = false;
    bool mSelected          = false;
    bool mHighLighted       = false;
    bool mHandleSelected    = false;
    bool mHandleHighLighted = false;
};

#endif // LABELRECT_H
