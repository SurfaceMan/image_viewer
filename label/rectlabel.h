#ifndef RECTLABEL_H
#define RECTLABEL_H

#include "label.h"

class RectLabel : public Label {
public:
    RectLabel();

    void onPaint(const PaintInfo &info) override;

    QRectF rect() const;
    void   setRect(const QRectF &value);

    QPen getOutlinePen(const PaintInfo &info) const;

private:
    QRectF mRect;
};

#endif // RECTLABEL_H
