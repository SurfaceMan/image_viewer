#ifndef POLYGONLABEL_H
#define POLYGONLABEL_H

#include "label.h"
#include <QPolygon>

class PolygonLabel : public Label {
public:
    PolygonLabel();

    void onPaint(const PaintInfo &info) override;

    QPolygonF polygon() const;
    void      setPolygon(const QPolygonF &value);

    QPen getOutlinePen(const PaintInfo &info) const;

private:
    QPolygonF mPolygon;
};

#endif // POLYGONLABEL_H
