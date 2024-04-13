#include "polygoneditor.h"
#include "utils.h"

PolygonEditor::PolygonEditor() {}

void PolygonEditor::onPaint(const PaintInfo &info) {
    mHandleDistance = mHandleDistanceBase / info.worldScale;

    info.painter->save();

    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);

    auto def   = category();
    auto color = def->color();
    color.setAlpha(50);
    info.painter->setBrush(QBrush(color));
    info.painter->drawPolygon(mPolygon);

    auto handleRadius = def->lineWidth() * 2. / info.worldScale;

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

QStringList PolygonEditor::serialize() const {
    return {};
}

void PolygonEditor::deserialize(const QStringList &source) {
    Q_UNUSED(source)
}

bool PolygonEditor::select(const QPointF &pos) {

    if (!isCreation()) {
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

void PolygonEditor::moving(const QPointF &curPos, const QPointF &lastPos) {
    if (mPolygon.empty()) {
        return;
    }

    if (isCreation()) {
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

void PolygonEditor::release() {
    if (mPressed) {
        mPressed        = false;
        mHandleSelected = false;
    }
}

void PolygonEditor::modify(const QPointF &pos) {
    if (isCreation()) {
        abortCreation();
        return;
    }

    if (mHandleHighLighted) {
        if (mPolygon.size() <= 3) {
            return;
        }

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

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        if (QLineF::BoundedIntersection == edge.intersects(edge2, nullptr)) {
#else
        if (QLineF::BoundedIntersection == edge.intersect(edge2, nullptr)) {
#endif
            mPolygon.insert(i + 1, pos);
            break;
        }
    }
}

QPolygonF PolygonEditor::polygon() const {
    return mPolygon;
}

void PolygonEditor::setPolygon(const QPolygonF &value) {
    mPolygon = value;
    if (!mPolygon.empty()) {
        abortCreation();
    }
}

QPen PolygonEditor::getOutlinePen(const PaintInfo &info) const {
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
