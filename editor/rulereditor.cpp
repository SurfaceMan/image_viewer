#include "rulereditor.h"
#include "utils.h"

RulerEditor::RulerEditor() = default;

void RulerEditor::onPaint(const PaintInfo &info) {
    mHandleDistance = mHandleDistanceBase / info.worldScale;

    info.painter->save();

    // line
    QLineF line(mStart, mEnd);
    auto   pen = getOutlinePen(info);
    info.painter->setPen(pen);
    info.painter->drawLine(line);

    // text
    auto font = info.painter->font();
    font.setPointSizeF(16. / info.worldScale);
    auto textPen = pen;
    textPen.setColor(category()->color());
    textPen.setStyle(Qt::SolidLine);
    info.painter->setPen(textPen);
    info.painter->setFont(font);
    info.painter->drawText(line.center(), QString::number(line.length(), 'f', 3));

    info.painter->setPen(pen);

    // handle
    const auto def          = category();
    const auto handleRadius = def->lineWidth() / info.worldScale;
    if (mSelected || mHighLighted) {
        if (mSelected) {
            info.painter->setBrush(Qt::white);
        }

        auto radius = handleRadius;
        if (mHandleHighLighted && START == mHandle) {
            radius *= 1.5;
        }
        info.painter->drawEllipse(mStart, radius, radius);

        radius = handleRadius;
        if (mHandleHighLighted && END == mHandle) {
            radius *= 1.5;
        }
        info.painter->drawEllipse(mEnd, radius, radius);
    }

    info.painter->restore();
}

bool RulerEditor::select(const QPointF &pos) {
    QLineF line(mStart, mEnd);

    if (!isCreation()) {
        mPressed = distance(pos, line) < mHandleDistance;

        if (distance(pos, mStart) < mHandleDistance) {
            mPressed        = true;
            mHandleSelected = true;
            mHandle         = START;
        }

        if (distance(pos, mEnd) < mHandleDistance) {
            mPressed        = true;
            mHandleSelected = true;
            mHandle         = END;
        }

        mSelected = mPressed;
        return mPressed;
    }

    if (line.isNull()) {
        mStart       = pos;
        mEnd         = pos + QPointF{1, 0};
        mPressed     = true;
        mSelected    = true;
        mHighLighted = true;
    } else {
        mEnd = pos;
        abortCreation();
    }

    return true;
}

void RulerEditor::moving(const QPointF &curPos, const QPointF &lastPos) {
    const QLineF line(mStart, mEnd);
    if (line.isNull()) {
        return;
    }

    if (isCreation()) {
        mEnd = curPos;
        return;
    }

    if (mPressed) {
        if (!mHandleSelected) {
            const auto delta  = curPos - lastPos;
            mStart           += delta;
            mEnd             += delta;
        } else {
            switch (mHandle) {
                case START: {
                    mStart = curPos;
                    break;
                }
                case END: {
                    mEnd = curPos;
                    break;
                }
            }
        }

        return;
    }

    mHighLighted = distance(curPos, line) < mHandleDistance;

    mHandleHighLighted = false;
    if (distance(curPos, mStart) < mHandleDistance) {
        mHighLighted       = true;
        mHandleHighLighted = true;
        mHandle            = START;
    }

    if (distance(curPos, mEnd) < mHandleDistance) {
        mHighLighted       = true;
        mHandleHighLighted = true;
        mHandle            = END;
    }
}

void RulerEditor::release() {
    if (mPressed) {
        mPressed        = false;
        mHandleSelected = false;
    }
}

QPen RulerEditor::getOutlinePen(const PaintInfo &info) const {
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
