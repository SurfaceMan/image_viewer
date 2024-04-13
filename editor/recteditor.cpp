#include "recteditor.h"
#include "utils.h"

RectEditor::RectEditor() {}

void RectEditor::onPaint(const PaintInfo &info) {
    mHandleDistance = mHandleDistanceBase / info.worldScale;
    mCrossLength    = mCrossLengthBase / info.worldScale;

    info.painter->save();

    // rect
    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);
    info.painter->drawRect(mRect);

    // center cross
    auto halfCrossLength = mCrossLength / 2.;
    auto center          = mRect.center();
    auto style           = pen.style();
    pen.setStyle(Qt::SolidLine);
    info.painter->setPen(pen);
    info.painter->drawLines({
        QPointF(center.x(), center.y() - halfCrossLength),
        QPointF(center.x(), center.y() + halfCrossLength),
        QPointF(center.x() - halfCrossLength, center.y()),
        QPointF(center.x() + halfCrossLength, center.y()),
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

        auto radius = handleRadius;
        if (mHandleHighLighted && mHandle == TOP_LEFT) {
            radius *= 1.5;
        }
        info.painter->drawEllipse(mRect.topLeft(), radius, radius);

        radius = handleRadius;
        if (mHandleHighLighted && mHandle == BOTTOM_RIGHT) {
            radius *= 1.5;
        }
        info.painter->drawEllipse(mRect.bottomRight(), radius, radius);
    }

    info.painter->restore();
}

QRectF generateRect(const QPointF &a, const QPointF &b) {
    auto delta = a - b;
    auto left  = delta.x() > 0 ? b.x() : a.x();
    auto top   = delta.y() > 0 ? b.y() : a.y();

    return {left, top, abs(delta.x()), abs(delta.y())};
}

bool RectEditor::select(const QPointF &pos) {
    if (!isCreation()) {
        // press check
        mPressed = mRect.contains(pos);

        // press handle check
        if (distance(mRect.topLeft(), pos) < mHandleDistance) {
            mPressed        = true;
            mHandleSelected = true;
            mHandle         = TOP_LEFT;
        } else if (distance(mRect.bottomRight(), pos) < mHandleDistance) {
            mPressed        = true;
            mHandleSelected = true;
            mHandle         = BOTTOM_RIGHT;
        }

        mSelected = mPressed;

        return mPressed;
    }

    if (mRect.isNull()) {
        mRect.setRect(pos.x(), pos.y(), 1, 1);
        mPressed     = true;
        mSelected    = true;
        mHighLighted = true;
    } else {
        mRect = generateRect(mRect.topLeft(), pos);
        abortCreation();
    }

    return true;
}

void RectEditor::moving(const QPointF &curPos, const QPointF &lastPos) {
    if (mRect.isNull()) {
        mRect.moveTopLeft(curPos);
        return;
    }

    if (isCreation()) {
        mRect = generateRect(mRect.topLeft(), curPos);
        return;
    }

    if (mPressed) {
        if (!mHandleSelected) {
            auto delta = curPos - lastPos;
            mRect.moveTopLeft(mRect.topLeft() + delta);
        } else {
            switch (mHandle) {
                case TOP_LEFT: {
                    mRect = generateRect(curPos, mRect.bottomRight());
                    break;
                }
                case BOTTOM_RIGHT: {
                    mRect = generateRect(mRect.topLeft(), curPos);
                    break;
                }
                default:
                    break;
            }
        }

        return;
    }

    mHighLighted = mRect.contains(curPos);

    mHandleHighLighted = false;
    if (distance(mRect.topLeft(), curPos) < mHandleDistance) {
        mHandleHighLighted = true;
        mHandle            = TOP_LEFT;
    }
    if (distance(mRect.bottomRight(), curPos) < mHandleDistance) {
        mHandleHighLighted = true;
        mHandle            = BOTTOM_RIGHT;
    }

    if (mHandleHighLighted)
        mHighLighted = true;
}

void RectEditor::release() {
    if (mPressed) {
        mPressed        = false;
        mHandleSelected = false;
    }
}

QRectF RectEditor::rect() const {
    return mRect;
}

void RectEditor::setRect(const QRectF &value) {
    mRect = value;
    if (!mRect.isNull()) {
        abortCreation();
    }
}

QPen RectEditor::getOutlinePen(const PaintInfo &info) const {
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
