#ifndef REGIONLABEL_H
#define REGIONLABEL_H

#include "label.h"

class RegionLabel : public Label {
public:
    RegionLabel();

    void onPaint(const PaintInfo &info) override;

    QVector<QPointF> region() const;
    void             setRegion(const QVector<QPointF> &value);

    QPen getOutlinePen(const PaintInfo &info) const;

private:
    QVector<QPointF> mRegion;
};

#endif // REGIONLABEL_H