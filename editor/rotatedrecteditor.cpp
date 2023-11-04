#include "rotatedrecteditor.h"
#include "utils.h"

#include <QTransform>

RotatedRectEditor::RotatedRectEditor() {
    mRectPoints.resize(5);
    mHandlePoints.resize(4);
}

void RotatedRectEditor::onPaint(const PaintInfo &info) {
    mHandleDistance = mHandleDistanceBase / info.worldScale;
    mCrossLength    = mCrossLengthBase / info.worldScale;

    info.painter->save();

    // rect
    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);
    info.painter->drawPolygon(mRectPoints);

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

        for (int i = 0; i < handleCount; i++) {
            auto radius = handleRadius;
            if (mHandleHighLighted && (Handle)i == mHandle) {
                radius *= 1.5;
            }

            info.painter->drawEllipse(mHandlePoints[ i ], radius, radius);
        }
    }

    info.painter->restore();
}

QRectF generateRect2(const QPointF &a, const QPointF &b) {
    auto delta = a - b;
    auto left  = delta.x() > 0 ? b.x() : a.x();
    auto top   = delta.y() > 0 ? b.y() : a.y();

    return {left, top, abs(delta.x()), abs(delta.y())};
}

bool RotatedRectEditor::select(const QPointF &pos) {
    if (!isCreation()) {
        // press check
        mPressed = mRectPoints.containsPoint(pos, Qt::FillRule::WindingFill);

        // press handle check
        for (int i = 0; i < handleCount; i++) {
            if (distance2(pos, mHandlePoints[ i ]) < mHandleDistance) {
                mPressed        = true;
                mHandleSelected = true;
                mHandle         = (Handle)i;
            }
        }

        mSelected = mPressed;
        return mPressed;
    }

    if (mRect.isNull()) {
        mRect.setRect(pos.x(), pos.y(), 1, 1);
        updateRotatedRect(mRect, mAngle);
        mPressed     = true;
        mSelected    = true;
        mHighLighted = true;
    } else {
        mRect = generateRect2(mRect.topLeft(), pos);
        updateRotatedRect(mRect, mAngle);
        abortCreation();
    }

    return true;
}

void RotatedRectEditor::moving(const QPointF &curPos, const QPointF &lastPos) {
    if (mRect.isNull()) {
        mRect.moveTopLeft(curPos);
        return;
    }

    if (isCreation()) {
        mRect = generateRect2(mRect.topLeft(), curPos);
        updateRotatedRect(mRect, mAngle);
        return;
    }

    if (mPressed) {
        if (!mHandleSelected) {
            auto delta = curPos - lastPos;
            mRect.moveTopLeft(mRect.topLeft() + delta);
            updateRotatedRect(mRect, mAngle);
        } else {

            QTransform trans;
            auto       center = mRect.center();
            trans.translate(center.x(), center.y());
            trans.rotate(-mAngle);
            trans.translate(-center.x(), -center.y());
            auto invert = trans.inverted();
            auto pos    = invert.map(curPos);

            switch (mHandle) {
                case BOTTOM: {
                    QSizeF size  = mRect.size();
                    auto   delta = pos.y() - mRect.bottom();
                    size.setHeight(size.height() + delta * 2);
                    QRectF rect(QPointF(mRect.left(), mRect.top() - delta), size);
                    updateRotatedRect(rect, mAngle);
                    break;
                }
                case TOP: {
                    QSizeF size  = mRect.size();
                    auto   delta = pos.y() - mRect.top();
                    size.setHeight(size.height() - delta * 2);
                    QRectF rect(QPointF(mRect.left(), mRect.top() + delta), size);
                    updateRotatedRect(rect, mAngle);
                    break;
                }
                case LEFT: {
                    QSizeF size  = mRect.size();
                    auto   delta = pos.x() - mRect.left();
                    size.setWidth(size.width() - delta * 2);
                    QRectF rect(QPointF(mRect.left() + delta, mRect.top()), size);
                    updateRotatedRect(rect, mAngle);
                    break;
                }
                case RIGHT: {
                    QSizeF size  = mRect.size();
                    auto   delta = pos.x() - mRect.right();
                    size.setWidth(size.width() + delta * 2);
                    QRectF rect(QPointF(mRect.left() - delta, mRect.top()), size);
                    updateRotatedRect(rect, mAngle);
                    break;
                }
                default:
                    break;
            }
        }

        return;
    }

    mHighLighted = mRectPoints.containsPoint(curPos, Qt::FillRule::WindingFill);

    mHandleHighLighted = false;
    for (int i = 0; i < handleCount; i++) {
        if (distance2(curPos, mHandlePoints[ i ]) < mHandleDistance) {
            mHandleHighLighted = true;
            mHandle            = (Handle)i;
        }
    }

    if (mHandleHighLighted)
        mHighLighted = true;
}

void RotatedRectEditor::release() {
    if (mPressed) {
        mPressed        = false;
        mHandleSelected = false;
    }
}

void RotatedRectEditor::rotate(double angleDelta) {
    updateRotatedRect(mRect, mAngle + angleDelta);
}

double RotatedRectEditor::angle() const {
    return mAngle;
}

QRectF RotatedRectEditor::rect() const {
    return mRect;
}

void RotatedRectEditor::setRotatedRect(const QRectF &rect, double angle) {
    updateRotatedRect(rect, angle);
    if (!mRect.isNull()) {
        abortCreation();
    }
}

void RotatedRectEditor::updatePoint() {
    mRectPoints[ 0 ] = mRect.topLeft();
    mRectPoints[ 1 ] = mRect.topRight();
    mRectPoints[ 2 ] = mRect.bottomRight();
    mRectPoints[ 3 ] = mRect.bottomLeft();
    mRectPoints[ 4 ] = mRect.topLeft();

    auto center             = mRect.center();
    auto halfHeight         = mRect.height() / 2.;
    auto halfWidth          = mRect.width() / 2.;
    mHandlePoints[ TOP ]    = QPointF(center.x(), center.y() - halfHeight);
    mHandlePoints[ BOTTOM ] = QPointF(center.x(), center.y() + halfHeight);
    mHandlePoints[ LEFT ]   = QPointF(center.x() - halfWidth, center.y());
    mHandlePoints[ RIGHT ]  = QPointF(center.x() + halfWidth, center.y());

    QTransform trans;
    trans.translate(center.x(), center.y());
    trans.rotate(-mAngle);
    trans.translate(-center.x(), -center.y());
    mRectPoints   = trans.map(mRectPoints);
    mHandlePoints = trans.map(mHandlePoints);
}

void RotatedRectEditor::updateRotatedRect(const QRectF &rect, double angle) {
    mRect  = rect;
    mAngle = angle;
    updatePoint();
}

QPen RotatedRectEditor::getOutlinePen(const PaintInfo &info) const {
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
