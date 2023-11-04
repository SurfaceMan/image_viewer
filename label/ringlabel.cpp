#include "ringlabel.h"
#include "utils.h"

#include <QPainterPath>

RingLabel::RingLabel() {}

void RingLabel::onPaint(const PaintInfo &info) {
    info.painter->save();

    // circle
    auto def   = category();
    auto color = def->color();
    color.setAlpha(50);
    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);
    info.painter->setBrush(QBrush(color));
    QPainterPath path;
    path.addEllipse(mCenter, mInsideRadius, mInsideRadius);
    path.addEllipse(mCenter, mOutsideRadius, mOutsideRadius);
    info.painter->drawPath(path);

    info.painter->restore();
}

double RingLabel::insideRadius() const {
    return mInsideRadius;
}

double RingLabel::outsideRadius() const {
    return mOutsideRadius;
}

QPointF RingLabel::center() const {
    return mCenter;
}

void RingLabel::setRing(const QPointF &center, double insideRadius, double outsideRadius) {
    mInsideRadius  = insideRadius;
    mOutsideRadius = outsideRadius;
    mCenter        = center;
}

QPen RingLabel::getOutlinePen(const PaintInfo &info) const {
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