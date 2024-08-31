#ifndef RULEREDITOR_H
#define RULEREDITOR_H

#include "labeleditor.h"

class RulerEditor : public LabelEditor {
public:
    RulerEditor();

    void onPaint(const PaintInfo &info) override;
    bool select(const QPointF &pos) override;
    void moving(const QPointF &curPos, const QPointF &lastPos) override;
    void release() override;

private:
    QPen getOutlinePen(const PaintInfo &info) const;

private:
    QPointF mStart;
    QPointF mEnd;

    enum Handle { START, END };
    Handle mHandle = START;

    const double mHandleDistanceBase = 3.;
    double       mHandleDistance     = 3.;

    bool mPressed           = false;
    bool mSelected          = false;
    bool mHighLighted       = false;
    bool mHandleSelected    = false;
    bool mHandleHighLighted = false;
};

#endif // RULEREDITOR_H
