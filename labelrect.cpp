#include "labelrect.h"
#include "utils.h"

LabelRect::LabelRect() {}

void LabelRect::onPaint(const PaintInfo &info) {
    mHandleDistance = mHandleDistanceBase / info.worldScale;

    info.painter->save();

    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);
    info.painter->drawRect(mRect);

    auto def          = getCategory();
    auto handleRadius = def->getLineWidth() * 2. / info.worldScale;

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

QStringList LabelRect::serialize() const {
    return {};
}

void LabelRect::deserialize(const QStringList &strs) {}

QRectF generateRect(const QPointF &a, const QPointF &b) {
    auto delta = a - b;
    auto left  = delta.x() > 0 ? b.x() : a.x();
    auto top   = delta.y() > 0 ? b.y() : a.y();

    return {left, top, abs(delta.x()), abs(delta.y())};
}

bool LabelRect::select(const QPointF &pos) {
    if (!mInCreation) {
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

void LabelRect::moving(const QPointF &curPos, const QPointF &lastPos) {

    if (mInCreation) {
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

void LabelRect::release() {
    if (mPressed) {
        mPressed        = false;
        mHandleSelected = false;
    }
}

QPen LabelRect::getOutlinePen(const PaintInfo &info) const {
    auto def = getCategory();
    if (!def) {
        return {};
    }

    auto color     = def->getColor();
    auto lineWidth = def->getLineWidth();
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
