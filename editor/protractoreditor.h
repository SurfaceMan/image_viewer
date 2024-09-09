#ifndef PROTRACTOR_EDITOR_H
#define PROTRACTOR_EDITOR_H

#include "labeleditor.h"

class ProtractorEditor : public LabelEditor {
public:
    ProtractorEditor();

    void onPaint(const PaintInfo &info) override;
    bool select(const QPointF &pos) override;
    void moving(const QPointF &curPos, const QPointF &lastPos) override;
    void release() override;

private:
    QPen getOutlinePen(const PaintInfo &info) const;

private:
    QPointF mPoints[ 3 ];

    enum Handle { CENTER, START, END, MAX_COUNT };
    Handle mHandle = CENTER;

    const double mHandleDistanceBase = 3.;
    double       mHandleDistance     = 3.;

    bool mPressed           = false;
    bool mSelected          = false;
    bool mHighLighted       = false;
    bool mHandleSelected    = false;
    bool mHandleHighLighted = false;
};

#endif // PROTRACTOR_EDITOR_H
