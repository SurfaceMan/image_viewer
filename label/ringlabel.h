#ifndef RINGLABEL_H
#define RINGLABEL_H

#include "label.h"

class RingLabel : public Label {
public:
    RingLabel();

    void onPaint(const PaintInfo &info) override;

    double  insideRadius() const;
    double  outsideRadius() const;
    QPointF center() const;
    void    setRing(const QPointF &center, double insideRadius, double outsideRadius);

    QPen getOutlinePen(const PaintInfo &info) const;

private:
    QPointF mCenter;
    double  mInsideRadius  = 0;
    double  mOutsideRadius = 0;
};

#endif // RINGLABEL_H
