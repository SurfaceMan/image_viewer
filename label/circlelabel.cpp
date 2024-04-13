#include "circlelabel.h"
#include "utils.h"

CircleLabel::CircleLabel() = default;

void CircleLabel::onPaint(const PaintInfo &info) {
    info.painter->save();

    // circle
    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);
    info.painter->drawEllipse(mCenter, mRadius, mRadius);

    info.painter->restore();
}

double CircleLabel::radius() const {
    return mRadius;
}

QPointF CircleLabel::center() const {
    return mCenter;
}

void CircleLabel::setCircle(const QPointF &center, double radius) {
    mRadius = radius;
    mCenter = center;
}

QPen CircleLabel::getOutlinePen(const PaintInfo &info) const {
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