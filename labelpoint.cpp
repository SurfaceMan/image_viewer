#include "labelpoint.h"
#include <cassert>

void LabelPoint::setPoints(QPolygonF points) {
    mPoints = std::move(points);
    mVectors.clear();
}

void LabelPoint::setPoints(QPolygonF points, QPolygonF vectors) {
    assert(points.size() == vectors.size());
    mPoints  = std::move(points);
    mVectors = std::move(vectors);
}

void LabelPoint::onPaint(const PaintInfo &info) {
    info.painter->save();

    auto   style = Qt::SolidLine;
    QColor color(Qt::red);
    QPen   pen(style);
    pen.setColor(color);
    pen.setWidthF(mLineWidth / info.worldScale);
    info.painter->setPen(pen);

    info.painter->drawPoints(mPoints);
    pen.setColor(category()->color());
    pen.setWidthF(1. / info.worldScale);
    info.painter->setPen(pen);

    if (mLastScale != info.worldScale) {
        mLastScale = info.worldScale;

        auto vectorLength = mVectorLength / info.worldScale;
        auto size         = mPoints.size();
        if (mLines.empty()) {
            for (int i = 0; i < size; i++) {
                auto &startPoint = mPoints[ i ];
                auto  endPoint   = startPoint + mVectors[ i ] * vectorLength;

                mLines.append(startPoint);
                mLines.append(endPoint);
            }
        } else {
            for (int i = 0; i < size; i++) {
                auto &startPoint = mPoints[ i ];
                auto  endPoint   = startPoint + mVectors[ i ] * vectorLength;

                mLines[ 2 * i + 1 ] = endPoint;
            }
        }
    }

    info.painter->drawLines(mLines);

    info.painter->restore();
}
