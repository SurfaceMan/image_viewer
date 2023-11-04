#include "circleeditor.h"
#include "utils.h"

CircleEditor::CircleEditor() {}

void CircleEditor::onPaint(const PaintInfo &info) {
    mHandleDistance = mHandleDistanceBase / info.worldScale;
    mCrossLength    = mCrossLengthBase / info.worldScale;

    info.painter->save();

    // circle
    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);
    info.painter->drawEllipse(mCenter, mRadius, mRadius);

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
    auto def          = category();
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

bool CircleEditor::select(const QPointF &pos) {

    if (!isCreation()) {
        // press check
        mPressed = distance2_sq(pos, mCenter) < mRadius * mRadius;

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

    if (mRadius <= 0) {
        updateCircle(pos, 1);
    } else {
        auto radius = distance2(pos, mCenter);
        updateCircle(mCenter, radius);
        abortCreation();
    }

    return true;
}

void CircleEditor::moving(const QPointF &curPos, const QPointF &lastPos) {
    // invalid
    if (mRadius <= 0) {
        updateCircle(curPos, mRadius);
        return;
    }

    if (isCreation()) {
        auto radius = distance2(curPos, mCenter);
        updateCircle(mCenter, radius);
        return;
    }

    if (mPressed) {
        if (!mHandleSelected) {
            auto delta = curPos - lastPos;
            updateCircle(mCenter + delta, mRadius);
        } else {
            auto radius = distance2(curPos, mCenter);
            updateCircle(mCenter, radius);
        }

        return;
    }

    mHighLighted       = distance2_sq(curPos, mCenter) < mRadius * mRadius;
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

void CircleEditor::release() {
    if (mPressed) {
        mPressed        = false;
        mHandleSelected = false;
    }
}

double CircleEditor::radius() const {
    return mRadius;
}

QPointF CircleEditor::center() const {
    return mCenter;
}

void CircleEditor::setCircle(const QPointF &center, double radius) {
    updateCircle(center, radius);
    if (radius > 0) {
        abortCreation();
    }
}

void CircleEditor::updateCircle(const QPointF &center, double radius) {
    mRadius = radius;
    mCenter = center;
    updateHandle();
}

void CircleEditor::updateHandle() {
    mHandles[ TOP ]    = QPointF(mCenter.x(), mCenter.y() - mRadius);
    mHandles[ BOTTOM ] = QPointF(mCenter.x(), mCenter.y() + mRadius);
    mHandles[ LEFT ]   = QPointF(mCenter.x() - mRadius, mCenter.y());
    mHandles[ RIGHT ]  = QPointF(mCenter.x() + mRadius, mCenter.y());
}

QPen CircleEditor::getOutlinePen(const PaintInfo &info) const {
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