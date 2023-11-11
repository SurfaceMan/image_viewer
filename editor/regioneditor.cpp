#include "regioneditor.h"

RegionEditor::RegionEditor() {}

RegionEditor::~RegionEditor() {}

void RegionEditor::onPaint(const PaintInfo &info) {
    info.painter->save();

    auto color = category()->color();
    QPen pen(color);
    pen.setWidth(1);

    if (mRenderedRegion.isNull()) {
        mRenderedRegion = QImage(info.size.toSize(), QImage::Format_ARGB32);
        mRenderedRegion.fill(Qt::transparent);

        mPainter.reset(new QPainter(&mRenderedRegion));
        mPainter->setPen(pen);
        mPainter->setCompositionMode(QPainter::CompositionMode_Source);
        mPainter->drawLines(mRegion);
    }

    // region
    info.painter->drawImage(info.offset, mRenderedRegion);

    if (isCreation()) {
        // indicator
        info.painter->setPen(pen);
        color.setAlpha(50);
        if (PEN == mTool) {
            info.painter->setBrush(color);
        }
        QRectF rect(mCenter.x() - mToolRadius, mCenter.y() - mToolRadius, mToolRadius * 2,
                    mToolRadius * 2);
        RECT == mToolShape ? info.painter->drawRect(rect) : info.painter->drawEllipse(rect);

        pen.setStyle(Qt::DotLine);
        pen.setWidthF(category()->lineWidth() / info.worldScale);
        info.painter->setPen(pen);
        info.painter->setBrush(Qt::NoBrush);

        info.painter->drawRect(QRectF(info.offset, info.size));
    }

    info.painter->restore();
}

bool RegionEditor::select(const QPointF &pos) {
    // press check
    if (!mRenderedRegion.valid(pos.x(), pos.y())) {
        mPressed = false;
        abortCreation();

        return mPressed;
    }

    mPressed = true;
    if (!isCreation()) {
        auto color  = mRenderedRegion.pixelColor(pos.x(), pos.y());
        auto alpha  = color.alpha();
        mPressed    = alpha != 0;
        mInCreation = mPressed;
        return mPressed;
    }

    auto color = category()->color();
    color.setAlpha(50);
    QPen pen(PEN == mTool ? color : Qt::transparent);
    pen.setWidth(1);

    // QPainter painter(&mRenderedRegion);
    mPainter->setBrush(PEN == mTool ? color : Qt::transparent);
    mPainter->setPen(pen);

    QRectF rect(mCenter.x() - mToolRadius, mCenter.y() - mToolRadius, mToolRadius * 2,
                mToolRadius * 2);
    RECT == mToolShape ? mPainter->drawRect(rect) : mPainter->drawEllipse(rect);

    return mPressed;
}

void RegionEditor::moving(const QPointF &curPos, const QPointF &lastPos) {
    mCenter = curPos;

    if (!mPressed) {
        return;
    }

    auto color = category()->color();
    color.setAlpha(50);
    QPen pen(PEN == mTool ? color : Qt::transparent);
    pen.setWidth(mToolRadius * 2);
    pen.setCapStyle(Qt::RoundCap);

    mPainter->setPen(pen);
    mPainter->drawLine(lastPos, curPos);
}

void RegionEditor::release() {
    mPressed = false;
}

void RegionEditor::modify(const QPointF &pos) {
    Q_UNUSED(pos);

    mTool = PEN == mTool ? ERASER : PEN;
}

void RegionEditor::rotate(double angleDelta) {
    mToolRadius += angleDelta * 2;
    if (mToolRadius < 1.) {
        mToolRadius = 1.;
    }
}

QVector<QPointF> RegionEditor::region() const {
    // render region to region
    QVector<QPointF> result;

    auto &img         = mRenderedRegion;
    int   startColumn = -1;
    int   endColumn   = -1;
    for (int i = 0; i < img.height(); i++) {
        for (int j = 0; j < img.width(); j++) {
            QColor color = img.pixelColor(i, j);

            if (color.alpha() == 0) {
                if (startColumn == -1) {
                    continue;
                }

                // insert
                result.append(QPointF(startColumn, i));
                result.append(QPointF(startColumn == endColumn ? endColumn + 0.001 : endColumn, i));

                startColumn = -1;
                endColumn   = -1;
            } else {
                if (startColumn == -1) {
                    startColumn = endColumn = j;
                } else {
                    endColumn = j;
                }

                if (j < img.width() - 1) {
                    continue;
                }

                // insert
                result.append(QPointF(startColumn, i));
                result.append(QPointF(startColumn == endColumn ? endColumn + 0.001 : endColumn, i));

                startColumn = -1;
                endColumn   = -1;
            }
        }
    }

    return result;
}

void RegionEditor::setReion(const QVector<QPointF> &value) {
    mRegion = value;

    // clear render region
    if (!mRenderedRegion.isNull()) {
        mRenderedRegion.fill(Qt::transparent);
    }
}

RegionEditor::Tool RegionEditor::tool() const {
    return mTool;
}

void RegionEditor::setTool(Tool tool) {
    mTool = tool;
}

double RegionEditor::toolRadius() const {
    return mToolRadius;
}

void RegionEditor::setToolRadius(double radius) {
    mToolRadius = radius;
}

RegionEditor::Shape RegionEditor::toolShape() const {
    return mToolShape;
}

void RegionEditor::setToolShape(Shape shape) {
    mToolShape = shape;
}