#include "protractoreditor.h"
#include "utils.h"

ProtractorEditor::ProtractorEditor() = default;

void ProtractorEditor::onPaint(const PaintInfo &info) {
    mHandleDistance = mHandleDistanceBase / info.worldScale;

    info.painter->save();

    auto pen = getOutlinePen(info);
    info.painter->setPen(pen);

    if (!isCreation()) {
        // line
        QLineF line1(mPoints[ CENTER ], mPoints[ START ]);
        QLineF line2(mPoints[ CENTER ], mPoints[ END ]);

        info.painter->drawLine(line1);
        info.painter->drawLine(line2);

        // text
        auto angle = line1.angleTo(line2);

        auto font = info.painter->font();
        font.setPointSizeF(16. / info.worldScale);
        auto textPen = pen;
        textPen.setColor(category()->color());
        textPen.setStyle(Qt::SolidLine);
        info.painter->setPen(textPen);
        info.painter->setFont(font);
        info.painter->drawText(mPoints[ CENTER ], QString::number(angle, 'f', 3));

        info.painter->setPen(pen);

        // handle
        const auto def          = category();
        const auto handleRadius = def->lineWidth() / info.worldScale;
        if (mSelected || mHighLighted) {
            if (mSelected) {
                info.painter->setBrush(Qt::white);
            }

            for (int i = 0; i < MAX_COUNT; i++) {
                auto radius = handleRadius;
                if (mHandleHighLighted && i == mHandle) {
                    radius *= 1.5;
                }
                info.painter->drawEllipse(mPoints[ i ], radius, radius);
            }
        }
    } else {
        switch (mHandle) {
            case CENTER: {
                break;
            }
            case START: {
                QLineF line1(mPoints[ CENTER ], mPoints[ START ]);
                info.painter->drawLine(line1);
                break;
            }
            case END: {
                QLineF line1(mPoints[ CENTER ], mPoints[ START ]);
                QLineF line2(mPoints[ CENTER ], mPoints[ END ]);

                info.painter->drawLine(line1);
                info.painter->drawLine(line2);

                // text
                auto angle = line1.angleTo(line2);

                auto font = info.painter->font();
                font.setPointSizeF(16. / info.worldScale);
                auto textPen = pen;
                textPen.setColor(category()->color());
                textPen.setStyle(Qt::SolidLine);
                info.painter->setPen(textPen);
                info.painter->setFont(font);
                info.painter->drawText(mPoints[ CENTER ], QString::number(angle, 'f', 3));

                info.painter->setPen(pen);
                break;
            }
            default:
                break;
        }

        // handle
        const auto def          = category();
        const auto handleRadius = def->lineWidth() / info.worldScale;

        info.painter->setBrush(Qt::white);

        for (int i = 0; i <= mHandle; i++) {
            auto radius = handleRadius;
            if (i == mHandle) {
                radius *= 1.5;
            }
            info.painter->drawEllipse(mPoints[ i ], radius, radius);
        }
    }

    info.painter->restore();
}

bool ProtractorEditor::select(const QPointF &pos) {
    if (!isCreation()) {
        mPressed = false;
        for (int i = 0; i < MAX_COUNT; i++) {
            if (distance(mPoints[ i ], pos) < mHandleDistance) {
                mPressed        = true;
                mHandleSelected = true;
                mHandle         = static_cast<Handle>(i);
                break;
            }
        }

        mSelected = mPressed;
        return mPressed;
    }

    if (mHandle < END) {
        mPressed           = true;
        mSelected          = true;
        mHighLighted       = true;
        mPoints[ mHandle ] = pos;

        mHandle            = CENTER == mHandle ? START : END;
        mPoints[ mHandle ] = pos;
    } else {
        mPoints[ mHandle ] = pos;
        abortCreation();
    }

    return true;
}

void ProtractorEditor::moving(const QPointF &curPos, const QPointF &lastPos) {
    if (isCreation()) {
        mPoints[ mHandle ] = curPos;
        return;
    }

    if (mPressed) {
        if (!mHandleSelected) {
            return;
        }

        mPoints[ mHandle ] = curPos;
        return;
    }

    for (int i = 0; i < MAX_COUNT; i++) {
        if (distance(mPoints[ i ], curPos) < mHandleDistance) {
            mHighLighted       = true;
            mHandleHighLighted = true;
            mHandle            = static_cast<Handle>(i);
            break;
        }
    }
}

void ProtractorEditor::release() {
    if (mPressed) {
        mPressed        = false;
        mHandleSelected = false;
    }
}

QPen ProtractorEditor::getOutlinePen(const PaintInfo &info) const {
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
