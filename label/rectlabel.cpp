#include "rectlabel.h"
#include "utils.h"

RectLabel::RectLabel() {}

void RectLabel::onPaint(const PaintInfo &info) {
    info.painter->save();

    // rect
    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);
    info.painter->drawRect(mRect);

    info.painter->restore();
}

QRectF RectLabel::rect() const {
    return mRect;
}

void RectLabel::setRect(const QRectF &value) {
    mRect = value;
}

QPen RectLabel::getOutlinePen(const PaintInfo &info) const {
    auto def = category();
    if (!def) {
        return {};
    }

    auto color     = def->color();
    auto lineWidth = def->lineWidth();
    auto style     = Qt::SolidLine;

    double width = abs(lineWidth) / info.worldScale;
    QPen   pen(style);
    pen.setColor(color);
    pen.setWidthF(width);

    return pen;
}
