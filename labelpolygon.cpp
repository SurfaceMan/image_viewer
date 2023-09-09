#include "labelpolygon.h"
#include "utils.h"

LabelPolygon::LabelPolygon() {}

void LabelPolygon::onPaint(const PaintInfo &info) {
    mHandleDistance = mHandleDistanceBase / info.worldScale;

    info.painter->save();

    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);

    auto def   = getCategory();
    auto color = def->getColor();
    color.setAlpha(50);
    info.painter->setBrush(QBrush(color));
    info.painter->drawPolygon(mPolygon);

    auto handleRadius = def->getLineWidth() * 2. / info.worldScale;

    if (mSelected || mHighLighted) {
        if (mSelected) {
            info.painter->setBrush(Qt::white);
        }

        for (int i = 0; i < mPolygon.size(); i++) {
            auto radius = handleRadius;
            if (mHandleHighLighted && mHandleIndex == i) {
                radius *= 1.5;
            }

            info.painter->drawEllipse(mPolygon[ i ], radius, radius);
        }
    }

    info.painter->restore();
}

QStringList LabelPolygon::serialize() const {
    return {};
}

void LabelPolygon::deserialize(const QStringList &strs) {}

bool LabelPolygon::select(const QPointF &pos) {

    if (!mInCreation) {
        // press check
        mPressed = mPolygon.containsPoint(pos, Qt::FillRule::WindingFill);

        // press handle check
        for (int i = 0; i < mPolygon.size(); i++) {
            if (distance(mPolygon[ i ], pos) < mHandleDistance) {
                mPressed        = true;
                mHandleSelected = true;
                mHandleIndex    = i;
                break;
            }
        }

        mSelected = mPressed;

        return mPressed;
    }

    if (mPolygon.isEmpty()) {
        mPolygon.append(pos);
        mPolygon.append(pos);
        mPressed     = true;
        mSelected    = true;
        mHighLighted = true;
    } else {
        mPolygon.insert(mPolygon.size() - 1, pos);
    }

    return true;
}

void LabelPolygon::moving(const QPointF &curPos, const QPointF &lastPos) {
    if (mInCreation) {
        mPolygon[ mPolygon.size() - 1 ] = curPos;
        return;
    }

    if (mPressed) {
        if (!mHandleSelected) {
            auto delta = curPos - lastPos;
            mPolygon.translate(delta);
        } else {
            mPolygon[ mHandleIndex ] = curPos;
        }
        return;
    }

    mHighLighted       = mPolygon.containsPoint(curPos, Qt::FillRule::WindingFill);
    mHandleHighLighted = false;
    for (int i = 0; i < mPolygon.size(); i++) {
        if (distance(mPolygon[ i ], curPos) < mHandleDistance) {
            mHandleHighLighted = true;
            mHandleIndex       = i;
            break;
        }
    }

    if (mHandleHighLighted)
        mHighLighted = true;
}

void LabelPolygon::release() {
    if (mPressed) {
        mPressed        = false;
        mHandleSelected = false;
    }
}

void LabelPolygon::modify(const QPointF &pos) {
    if (mHandleHighLighted) {
        // remove handle
        mPolygon.removeAt(mHandleIndex);
        return;
    }

    for (int i = 0; i < mPolygon.size(); i++) {
        // add handle
        auto point0 = mPolygon[ i ];
        auto point1 = mPolygon[ (i + 1) % mPolygon.size() ];

        QLineF edge(point0, point1);
        QLineF edge2(pos, pos + QPointF(mHandleDistance, 0));
        edge2.setAngle(edge.angle() + 90);
        edge2.setP1(edge2.p1() + edge2.p1() - edge2.p2());

        if (QLineF::BoundedIntersection == edge.intersects(edge2, nullptr)) {
            mPolygon.insert(i + 1, pos);
            break;
        }
    }
}

QPen LabelPolygon::getOutlinePen(const PaintInfo &info) const {
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
    pen.setWidth(width);
    pen.setCosmetic(lineWidth < 0);

    return pen;
}
