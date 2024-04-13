#ifndef CIRCLELABEL_H
#define CIRCLELABEL_H

#include "label.h"

class CircleLabel : public Label {
public:
    CircleLabel();

    void onPaint(const PaintInfo &info) override;

    double  radius() const;
    QPointF center() const;
    void    setCircle(const QPointF &center, double radius);

private:
    QPen getOutlinePen(const PaintInfo &info) const;

private:
    QPointF mCenter;
    double  mRadius = 0;
};

#endif // CIRCLELABEL_H
