#include "regionlabel.h"

RegionLabel::RegionLabel() = default;

void RegionLabel::onPaint(const PaintInfo &info) {
    info.painter->save();

    // rect
    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);
    info.painter->drawLines(mRegion);

    info.painter->restore();
}

QVector<QPointF> RegionLabel::region() const {
    return mRegion;
}

void RegionLabel::setRegion(const QVector<QPointF> &value) {
    mRegion = value;
}

QPen RegionLabel::getOutlinePen(const PaintInfo &info) const {
    Q_UNUSED(info)

    auto def = category();
    if (!def) {
        return {};
    }

    auto color = def->color();
    color.setAlpha(50);
    auto style = Qt::SolidLine;

    QPen pen(style);
    pen.setColor(color);
    pen.setWidth(1);

    return pen;
}