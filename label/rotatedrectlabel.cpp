#include "rotatedrectlabel.h"
#include "utils.h"

#include <QTransform>

RotatedRectLabel::RotatedRectLabel() {
    mRectPoints.resize(5);
}

void RotatedRectLabel::onPaint(const PaintInfo &info) {
    info.painter->save();

    // rect
    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);
    info.painter->drawPolygon(mRectPoints);

    info.painter->restore();
}

double RotatedRectLabel::angle() const {
    return mAngle;
}

QRectF RotatedRectLabel::rect() const {
    return mRect;
}

void RotatedRectLabel::setRotatedRect(const QRectF &rect, double angle) {
    updateRotatedRect(rect, angle);
}

void RotatedRectLabel::updatePoint() {
    mRectPoints[ 0 ] = mRect.topLeft();
    mRectPoints[ 1 ] = mRect.topRight();
    mRectPoints[ 2 ] = mRect.bottomRight();
    mRectPoints[ 3 ] = mRect.bottomLeft();
    mRectPoints[ 4 ] = mRect.topLeft();

    auto center = mRect.center();

    QTransform trans;
    trans.translate(center.x(), center.y());
    trans.rotate(-mAngle);
    trans.translate(-center.x(), -center.y());
    mRectPoints = trans.map(mRectPoints);
}

void RotatedRectLabel::updateRotatedRect(const QRectF &rect, double angle) {
    mRect  = rect;
    mAngle = angle;
    updatePoint();
}

QPen RotatedRectLabel::getOutlinePen(const PaintInfo &info) const {
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
