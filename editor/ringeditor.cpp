#include "ringeditor.h"
#include "utils.h"

#include <QPainterPath>

RingEditor::RingEditor() = default;

void RingEditor::onPaint(const PaintInfo &info) {
    mHandleDistance = mHandleDistanceBase / info.worldScale;
    mCrossLength    = mCrossLengthBase / info.worldScale;

    info.painter->save();

    // circle

    auto def   = category();
    auto color = def->color();
    color.setAlpha(100);
    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);
    info.painter->setBrush(QBrush(color));
    QPainterPath path;
    path.addEllipse(mCenter, mInsideRadius, mInsideRadius);
    path.addEllipse(mCenter, mOutsideRadius, mOutsideRadius);
    info.painter->drawPath(path);

    // center cross
    auto halfCrossLength = mCrossLength / 2.;
    auto style           = pen.style();
    pen.setStyle(Qt::SolidLine);
    info.painter->setPen(pen);
    info.painter->drawLines({
        QPointF(mCenter.x(), mCenter.y() - halfCrossLength),
        QPointF(mCenter.x(), mCenter.y() + halfCrossLength),
        QPointF(mCenter.x() - halfCrossLength, mCenter.y()),
        QPointF(mCenter.x() + halfCrossLength, mCenter.y()),
    });
    pen.setStyle(style);
    info.painter->setPen(pen);

    // handle
    auto handleRadius = def->lineWidth() * 2. / info.worldScale;

    if (mSelected || mHighLighted) {
        if (mSelected) {
            info.painter->setBrush(Qt::white);
        }

        for (int i = 0; i < handleCount; i++) {
            auto radius = handleRadius;
            if (mHandleHighLighted && (Handle)i == mHandle) {
                radius *= 1.5;
            }

            info.painter->drawEllipse(mHandles[ i ], radius, radius);
        }
    }

    info.painter->restore();
}

bool RingEditor::select(const QPointF &pos) {

    if (!isCreation()) {
        // press check
        mPressed = distance2_sq(pos, mCenter) < mOutsideRadius * mOutsideRadius;

        for (int i = 0; i < handleCount; i++) {
            if (distance2(pos, mHandles[ i ]) < mHandleDistance) {
                mPressed        = true;
                mHandleSelected = true;
                mHandle         = (Handle)i;
            }
        }

        mSelected = mPressed;
        return mPressed;
    }

    if (mInsideRadius <= 0) {
        updateRing(pos, 1, 1);
    } else {
        auto radius = distance2(pos, mCenter);
        if (mInsideRadius == mOutsideRadius) {
            updateRing(mCenter, radius, radius + 1);
        } else if (radius > mInsideRadius) {
            updateRing(mCenter, mInsideRadius, radius);
            abortCreation();
        }
    }

    return true;
}

void RingEditor::moving(const QPointF &curPos, const QPointF &lastPos) {
    // invalid
    if (mInsideRadius <= 0) {
        updateRing(curPos, mInsideRadius, mOutsideRadius);
        return;
    }

    if (isCreation()) {
        auto radius = distance2(curPos, mCenter);
        if (mInsideRadius == mOutsideRadius) {
            updateRing(mCenter, radius, radius);
        } else if (radius > mInsideRadius) {
            updateRing(mCenter, mInsideRadius, radius);
        }
        return;
    }

    if (mPressed) {
        if (!mHandleSelected) {
            auto delta = curPos - lastPos;
            updateRing(mCenter + delta, mInsideRadius, mOutsideRadius);
        } else {
            auto radius = distance2(curPos, mCenter);
            if (radius < 1)
                radius = 1.1;

            if (INSIDE == mHandle) {
                updateRing(mCenter, radius, radius > mOutsideRadius ? radius + 1 : mOutsideRadius);
            } else {
                updateRing(mCenter, radius < mInsideRadius ? radius - 1 : mInsideRadius, radius);
            }
        }

        return;
    }

    mHighLighted       = distance2_sq(curPos, mCenter) < mOutsideRadius * mOutsideRadius;
    mHandleHighLighted = false;
    for (int i = 0; i < handleCount; i++) {
        if (distance2(curPos, mHandles[ i ]) < mHandleDistance) {
            mHandleHighLighted = true;
            mHandle            = (Handle)i;
        }
    }

    if (mHandleHighLighted) {
        mHighLighted = true;
    }
}

void RingEditor::release() {
    if (mPressed) {
        mPressed        = false;
        mHandleSelected = false;
    }
}

double RingEditor::insideRadius() const {
    return mInsideRadius;
}

double RingEditor::outsideRadius() const {
    return mOutsideRadius;
}

QPointF RingEditor::center() const {
    return mCenter;
}

void RingEditor::setRing(const QPointF &center, double insideRadius, double outsideRadius) {
    updateRing(center, insideRadius, outsideRadius);
    if (insideRadius > 0 && outsideRadius > insideRadius) {
        abortCreation();
    }
}

void RingEditor::updateRing(const QPointF &center, double insideRadius, double outsideRadius) {
    mInsideRadius  = insideRadius;
    mOutsideRadius = outsideRadius;
    mCenter        = center;
    updateHandle();
}

void RingEditor::updateHandle() {
    mHandles[ INSIDE ]  = QPointF(mCenter.x() + mInsideRadius, mCenter.y());
    mHandles[ OUTSIDE ] = QPointF(mCenter.x() + mOutsideRadius, mCenter.y());
}

QPen RingEditor::getOutlinePen(const PaintInfo &info) const {
    auto def = category();
    if (!def) {
        return {};
    }

    auto color     = def->color();
    auto lineWidth = def->lineWidth();
    auto style     = Qt::SolidLine;

    if (mSelected) {
        color.setAlpha(150);
    }
    if (mHighLighted) {
        style = Qt::DotLine;
    }

    double width = abs(lineWidth) / info.worldScale;
    QPen   pen(style);
    pen.setColor(color);
    pen.setWidthF(width);

    return pen;
}