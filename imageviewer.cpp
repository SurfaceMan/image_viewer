#include "imageviewer.h"
#include "labeleditor.h"
#include "labelfactory.h"
#include "types.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <limits>

ImageViewer::ImageViewer(QWidget *parent)
    : QWidget{parent} {
    setMouseTracking(true);

    setBackgroundRole(QPalette::Mid);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::ClickFocus);

    mBackground = QImage(":/mask.png");
}

void ImageViewer::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    painter.fillRect(rect(), QBrush(mBackground));

    if (mImg.isNull()) {
        return;
    }

    // render image with half pixel offset
    QTransform halfPixel;
    halfPixel.translate(mImageOriginOffset.x(), mImageOriginOffset.y());
    painter.setTransform(halfPixel * getWorldTransform());
    painter.drawImage(0, 0, mImg);

    // Setup world transform matrix
    painter.setTransform(getWorldTransform());
    painter.setRenderHint(QPainter::Antialiasing);

    PaintInfo info;
    info.painter    = &painter;
    info.worldScale = getWorldScale();
    foreach (auto label, mLabels) {
        label->onPaint(info);
    }
}

void ImageViewer::mousePressEvent(QMouseEvent *event) {
    setMousePos(event);

    if (mImg.isNull()) {
        return;
    }

    if (event->button() == Qt::LeftButton && QApplication::keyboardModifiers() == Qt::NoModifier) {
        if (mInPixelSelect) {
            // check valid position
            auto pos = mMousePos.toPoint();
            if (pos.x() >= 0 && pos.x() < mImg.width() && pos.y() >= 0 && pos.y() < mImg.height()) {
                auto color = mImg.pixelColor(pos);
                emit pixelValueChanged(pos, color);
            } else {
                emit pixelValueChanged(pos, QColor());
            }
        } else {
            auto pos            = mMousePos;
            mSelectedLabelIndex = INVALID_INDEX;
            for (int i = 0; i < mLabels.size(); i++) {
                auto *editor = dynamic_cast<LabelEditor *>(mLabels[ i ].get());
                if (nullptr != editor && editor->select(pos)) {
                    mSelectedLabelIndex = i;

                    // unselect other label
                    pos.setX(std::numeric_limits<float>::max());
                }
            }
        }

    } else if (event->button() == Qt::RightButton) {
        // context menu
    }

    update();
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *event) {
    setMousePos(event);

    if (mImg.isNull()) {
        return;
    }

    if (mSelectedLabelIndex == INVALID_INDEX) {
        return;
    }

    auto *editor = dynamic_cast<LabelEditor *>(mLabels[ mSelectedLabelIndex ].get());
    if (event->button() == Qt::LeftButton) {
        editor->release();
    } else if (event->button() == Qt::RightButton) {
        if (editor->isCreation()) {
            // abort creation
            editor->abortCreation();
            // mSelectedLabelIndex = INVALID_INDEX;
        } else {
            // modify
            editor->modify(mMousePos);
        }
    }

    update();
}

void ImageViewer::mouseMoveEvent(QMouseEvent *event) {
    auto oldMousePos = mMousePos;
    setMousePos(event);

    auto oldPos     = mMousePosPixels;
    mMousePosPixels = event->pos();

    if (mImg.isNull()) {
        return;
    }

    if (event->buttons() == Qt::NoButton) {
        foreach (auto label, mLabels) {
            auto *editor = dynamic_cast<LabelEditor *>(label.get());
            if (nullptr != editor) {
                editor->moving(mMousePos, oldMousePos);
            }
        }
    }

    if (mSelectedLabelIndex != INVALID_INDEX && (event->buttons() & Qt::LeftButton)) {
        auto *editor = dynamic_cast<LabelEditor *>(mLabels[ mSelectedLabelIndex ].get());
        editor->moving(mMousePos, oldMousePos);
        update();
        return;
    }

    if ((event->buttons() & Qt::MiddleButton) || (event->buttons() & Qt::LeftButton)) {
        auto delta          = oldPos - mMousePosPixels;
        mWorldOffset       -= QPointF(delta.x() / mWorldScale, delta.y() / mWorldScale);
        mFitToViewOnResize  = false;
    }

    update();
}

void ImageViewer::wheelEvent(QWheelEvent *event) {
    if (mImg.isNull()) {
        return;
    }

    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        double scale         = mWorldScale * pow(mScaleFactor, event->angleDelta().y() / 240.);
        auto   oldWorldScale = mWorldScale;
        setWorldScale(scale);

        float deltaX =
            (mWorldOffset.x() * oldWorldScale + mMousePos.x() * (oldWorldScale - mWorldScale)) /
            mWorldScale;
        float deltaY =
            (mWorldOffset.y() * oldWorldScale + mMousePos.y() * (oldWorldScale - mWorldScale)) /
            mWorldScale;

        mWorldOffset.setX(deltaX);
        mWorldOffset.setY(deltaY);

        mFitToViewOnResize = false;
    }

    update();
}

void ImageViewer::keyPressEvent(QKeyEvent *event) {
    if (mImg.isNull()) {
        return;
    }

    if (event->key() == Qt::Key_Delete) {
        if (mSelectedLabelIndex == INVALID_INDEX) {
            return;
        }

        mLabels.removeAt(mSelectedLabelIndex);
        mSelectedLabelIndex = INVALID_INDEX;
    }

    update();
}

void ImageViewer::resizeEvent(QResizeEvent *event) {
    if (mFitToViewOnResize) {
        fitToView();
    }

    QWidget::resizeEvent(event);
}

void ImageViewer::resetWorldTransform() {
    setWorldScale(1.0);
    mWorldOffset = {0, 0};
}

void ImageViewer::setWorldScaleAndUpdate(double value) {
    auto oldWorldScale = mWorldScale;
    auto centerPos =
        getWorldTransform().inverted().map(QPointF(this->width() / 2, this->height() / 2));

    setWorldScale(value);

    const auto deltaX =
        (mWorldOffset.x() * oldWorldScale + centerPos.x() * (oldWorldScale - mWorldScale)) /
        mWorldScale;
    const auto deltaY =
        (mWorldOffset.y() * oldWorldScale + centerPos.y() * (oldWorldScale - mWorldScale)) /
        mWorldScale;

    mWorldOffset.setX(deltaX);
    mWorldOffset.setY(deltaY);

    mFitToViewOnResize = false;

    update();
}

void ImageViewer::zoomIn() {
    setWorldScaleAndUpdate(getWorldScale() + mScaleStep);
}

void ImageViewer::zoomOut() {
    setWorldScaleAndUpdate(getWorldScale() - mScaleStep);
}

void ImageViewer::fitToView() {
    if (mImg.isNull()) {
        resetWorldTransform();
    } else {
        auto scaleX = double(width()) / mImg.width();
        auto scaleY = double(height()) / mImg.height();

        setWorldScale(std::min(scaleX, scaleY));
        mWorldOffset = {(width() / mWorldScale - mImg.width()) / 2,
                        (height() / mWorldScale - mImg.height()) / 2};
    }

    mFitToViewOnResize = true;
    update();
}

void ImageViewer::resetToOriginalSize() {
    setWorldScaleAndUpdate(1.0);
}

QTransform ImageViewer::getWorldTransform() const {
    QTransform scale;
    QTransform translate;
    scale.scale(mWorldScale, mWorldScale);
    translate.translate(mWorldOffset.x(), mWorldOffset.y());
    return translate * scale;
}

void ImageViewer::setWorldScale(double value) {
    if (mMaxScale <= value || mMinScale >= value) {
        return;
    }

    mWorldScale = value;
    emit scaleFactorChanged(mWorldScale);
}

double ImageViewer::getWorldScale() const {
    return mWorldScale;
}

QPointF ImageViewer::getMousePos() {
    return mMousePos;
}

void ImageViewer::setMousePos(QMouseEvent *event) {
    mMousePos = getWorldTransform().inverted().map(QPointF(event->pos()));
}

void ImageViewer::loadImage(const QString &filepath) {
    const QImage img(filepath);
    setImage(img);
}

void ImageViewer::setImage(const QImage &img_) {
    if (img_.isNull()) {
        return;
    }

    mImg = img_;
    emit imageSizeChanged(mImg.size());

    if (mFitToViewOnLoad) {
        fitToView();
    }

    update();
}

const QImage ImageViewer::image() {
    return mImg;
}

void ImageViewer::addLabel(const QSharedPointer<Label> &label) {
    mLabels.append(label);
    update();
}

void ImageViewer::removeLabel(const QSharedPointer<Label> &label) {
    mLabels.removeAll(label);
    update();
}

void ImageViewer::clearLabel() {
    mLabels.clear();
    update();
}

void ImageViewer::setInSelect(bool pixelSelect) {
    mInPixelSelect = pixelSelect;
}
