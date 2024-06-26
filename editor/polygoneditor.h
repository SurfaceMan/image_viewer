#ifndef POLYGONEDITOR_H
#define POLYGONEDITOR_H

#include "labeleditor.h"
#include <QPolygon>

class PolygonEditor : public LabelEditor {
public:
    PolygonEditor();

    void        onPaint(const PaintInfo &info) override;
    QStringList serialize() const override;
    void        deserialize(const QStringList &source) override;

    bool select(const QPointF &pos) override;
    void moving(const QPointF &curPos, const QPointF &lastPos) override;
    void release() override;
    void modify(const QPointF &pos) override;

    QPolygonF polygon() const;
    void      setPolygon(const QPolygonF &value);

    QPen getOutlinePen(const PaintInfo &info) const;

private:
    QPolygonF mPolygon;

    int          mHandleIndex        = 0;
    const double mHandleDistanceBase = 3.;
    double       mHandleDistance     = 3.;

    bool mPressed           = false;
    bool mSelected          = false;
    bool mHighLighted       = false;
    bool mHandleSelected    = false;
    bool mHandleHighLighted = false;
};

#endif // POLYGONEDITOR_H
