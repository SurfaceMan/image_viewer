#include "polygonlabel.h"
#include "utils.h"

PolygonLabel::PolygonLabel() = default;

void PolygonLabel::onPaint(const PaintInfo &info) {
    info.painter->save();

    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);

    auto def   = category();
    auto color = def->color();
    color.setAlpha(50);
    info.painter->setBrush(QBrush(color));
    info.painter->drawPolygon(mPolygon);

    info.painter->restore();
}

QPolygonF PolygonLabel::polygon() const {
    return mPolygon;
}

void PolygonLabel::setPolygon(const QPolygonF &value) {
    mPolygon = value;
}

QPen PolygonLabel::getOutlinePen(const PaintInfo &info) const {
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
