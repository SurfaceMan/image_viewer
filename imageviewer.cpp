#include "imageviewer.h"
#include "label/imagelabel.h"
#include "types.h"

#include <QApplication>
#include <QDebug>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QMenu>
#include <QPainter>
#include <limits>
#include <cmath>

ImageViewer::ImageViewer(QWidget *parent)
    : QWidget{parent} {
    setMouseTracking(true);

    setBackgroundRole(QPalette::Mid);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::ClickFocus);

    mBackground = QImage(":/mask.png");
}

void ImageViewer::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter painter(this);

    painter.fillRect(rect(), QBrush(mBackground));
    painter.save();

    PaintInfo info;
    info.painter    = &painter;
    info.size       = mImageLabel ? mImageLabel->image().size() : QSizeF{0, 0};
    info.worldScale = getWorldScale();
    info.offset     = mImageOriginOffset;

    // render image with half pixel offset
    QTransform halfPixel;
    halfPixel.translate(mImageOriginOffset.x(), mImageOriginOffset.y());
    painter.setTransform(halfPixel * getWorldTransform());
    if (mImageLabel && mImageLabel->category()->visible()) {
        mImageLabel->onPaint(info);
    }

    // Setup world transform matrix
    painter.setTransform(getWorldTransform());
    painter.setRenderHint(QPainter::Antialiasing);
    foreach (auto &label, mLabels) {
        if (!label->category()->visible()) {
            continue;
        }
        label->onPaint(info);
    }

    foreach (auto &editor, mEditors) {
        if (!editor->category()->visible()) {
            continue;
        }
        editor->onPaint(info);
    }

    painter.restore();
    displayInfo(painter);
}

void ImageViewer::mousePressEvent(QMouseEvent *event) {
    setMousePos(event);

    if (event->button() != Qt::LeftButton || QApplication::keyboardModifiers() != Qt::NoModifier) {
        return;
    }

    if (mInPixelSelect) {
        auto pos = mMousePos.toPoint();
        if (!mImageLabel || mImageLabel->image().isNull()) {
            mSelectedColor = QColor();
        } else {
            // check valid position
            const auto &img = mImageLabel->image();
            if (pos.x() >= 0 && pos.x() < img.width() && pos.y() >= 0 && pos.y() < img.height()) {
                mSelectedColor = img.pixelColor(pos);
            } else {
                mSelectedColor = QColor();
            }
        }

        emit pixelValueChanged(pos, mSelectedColor);
    } else {
        if (mSelectedEditor && mSelectedEditor->isCreation()) {
            mSelectedEditor->select(mMousePos);
            update();
            return;
        }

        auto pos = mMousePos;
        mSelectedEditor.reset();
        for (auto i = mEditors.size() - 1; i >= 0; i--) {
            auto &editor = mEditors[ i ];
            if (editor->category()->visible() && editor->select(pos)) {
                mSelectedEditor = editor;

                // unselect other label
                pos.setX(std::numeric_limits<float>::max());
            }
        }
    }

    update();
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *event) {
    setMousePos(event);

    if (!mSelectedEditor) {
        return;
    }

    if (event->button() == Qt::LeftButton) {
        mSelectedEditor->release();
    } else if (event->button() == Qt::RightButton) {
        // modify
        mSelectedEditor->modify(mMousePos);
    }

    update();
}

void ImageViewer::mouseMoveEvent(QMouseEvent *event) {
    auto oldMousePos = mMousePos;
    setMousePos(event);

    auto oldPos     = mMousePosPixels;
    mMousePosPixels = event->pos();

    // highlight
    if (event->buttons() == Qt::NoButton) {
        foreach (auto editor, mEditors) {
            if (!editor->category()->visible()) {
                continue;
            }
            editor->moving(mMousePos, oldMousePos);
        }
    }

    if (mSelectedEditor && (event->buttons() & Qt::LeftButton)) {
        mSelectedEditor->moving(mMousePos, oldMousePos);
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
    if (mSelectedEditor && QApplication::keyboardModifiers() == Qt::NoModifier) {
        auto delta = event->angleDelta().y() / 128.;
        mSelectedEditor->rotate(delta);
        update();
        return;
    }

    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        double scale         = mWorldScale * pow(mScaleFactor, event->angleDelta().y() / 240.);
        auto   oldWorldScale = mWorldScale;
        setWorldScale(scale);

        auto deltaX =
            (mWorldOffset.x() * oldWorldScale + mMousePos.x() * (oldWorldScale - mWorldScale)) /
            mWorldScale;
        auto deltaY =
            (mWorldOffset.y() * oldWorldScale + mMousePos.y() * (oldWorldScale - mWorldScale)) /
            mWorldScale;

        mWorldOffset.setX(deltaX);
        mWorldOffset.setY(deltaY);

        mFitToViewOnResize = false;
    }

    update();
}

void ImageViewer::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete) {
        if (!mSelectedEditor) {
            return;
        }

        mEditors.removeAll(mSelectedEditor);
        mSelectedEditor.reset();
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
    auto centerPos     = getWorldTransform().inverted().map(QPointF(width() / 2., height() / 2.));

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
    if (!mImageLabel || mImageLabel->image().isNull()) {
        resetWorldTransform();
    } else {
        const auto &img    = mImageLabel->image();
        auto        scaleX = double(width()) / img.width();
        auto        scaleY = double(height()) / img.height();

        setWorldScale(std::min(scaleX, scaleY));
        mWorldOffset = {(width() / mWorldScale - img.width()) / 2,
                        (height() / mWorldScale - img.height()) / 2};
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

void ImageViewer::displayInfo(QPainter &painter) {
    painter.save();

    QImage      empty;
    const auto &img =
        (mImageLabel && !mImageLabel->image().isNull()) ? mImageLabel->image() : empty;

    auto str1 = (img.isNull() ? "" : QString("%1x%2 ").arg(img.width()).arg(img.height())) +
                QString::number(mWorldScale * 100.f, 'f', 2) + "%";
    auto str2 = QString("%1,%2").arg(mMousePos.x()).arg(mMousePos.y());
    auto str3 = img.format() != QImage::Format_Grayscale8
                    ? QString("R:%1,G:%2,B:%3")
                          .arg(mSelectedColor.red())
                          .arg(mSelectedColor.green())
                          .arg(mSelectedColor.blue())
                    : QString("L:%1").arg(mSelectedColor.lightness());

    QFontMetrics fm(painter.font());
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    int width1 = fm.boundingRect(str1).width();
    int width2 = fm.boundingRect(str2).width();
    int width3 = fm.boundingRect(str3).width();
#else
    int width1 = fm.width(str1);
    int width2 = fm.width(str2);
    int width3 = fm.width(str3);
#endif

    int height1 = fm.height();
    int width   = width1 > width2 ? width1 : width2;
    int height  = height1 * 2;
    if (mInPixelSelect) {
        width  = width > width3 ? width : width3;
        height = height1 * 3;
    }

    painter.setPen(QPen(Qt::transparent));
    painter.setBrush(QColor(35, 35, 35, 100));
    painter.drawRect(QRect(0, 0, width + 5, height + 5));
    painter.setPen(QPen(QColor(150, 250, 150)));
    painter.drawText(2, height1, str1);
    painter.drawText(2, height1 * 2, str2);
    if (mInPixelSelect) {
        painter.drawText(2, height1 * 3, str3);
    }

    painter.restore();
}

void ImageViewer::loadImage(const QString &filepath) {
    const QImage img(filepath);
    setImage(img);
}

void ImageViewer::setImage(const QImage &img_) {
    if (img_.isNull()) {
        return;
    }

    mImageLabel.reset(new ImageLabel);
    mImageLabel->setImage(img_);

    emit imageSizeChanged(img_.size());

    if (mFitToViewOnLoad) {
        fitToView();
    }

    update();
}

QImage ImageViewer::image() const {
    if (!mImageLabel || mImageLabel->image().isNull()) {
        return {};
    }

    return mImageLabel->image();
}

QImage ImageViewer::rendering() const {
    if (!mImageLabel || mImageLabel->image().isNull()) {
        return {};
    }

    auto &img = mImageLabel->image();

    QPixmap  target = QPixmap::fromImage(img, Qt::ColorOnly);
    QPainter painter(&target);

    PaintInfo info;
    info.painter    = &painter;
    info.worldScale = 1.;
    info.size       = img.size();
    info.offset     = {0, 0};
    foreach (auto &label, mLabels) {
        if (!label->category()->visible()) {
            continue;
        }
        label->onPaint(info);
    }
    foreach (auto &editor, mEditors) {
        if (!editor->category()->visible()) {
            continue;
        }
        editor->onPaint(info);
    }

    return target.toImage();
}

void ImageViewer::addLabel(const QSharedPointer<Label> &label) {
    if (!label) {
        return;
    }

    auto imageLabel = label.dynamicCast<ImageLabel>();
    if (imageLabel) {
        mImageLabel = imageLabel;
    } else {
        mLabels.append(label);
    }

    update();
}

void ImageViewer::removeLabel(const QSharedPointer<Label> &label) {
    mLabels.removeAll(label);
    if (mImageLabel == label) {
        mImageLabel.reset();
    }

    update();
}

void ImageViewer::clearLabel() {
    mLabels.clear();
    mImageLabel.reset();
    update();
}

void ImageViewer::addEditor(const QSharedPointer<LabelEditor> &editor) {
    if (!editor) {
        return;
    }

    mEditors.append(editor);
    if (editor->isCreation()) {
        mSelectedEditor = editor;
    }
    update();
}

void ImageViewer::removeEditor(const QSharedPointer<LabelEditor> &editor) {
    mEditors.removeAll(editor);
    mSelectedEditor.reset();
    update();
}

void ImageViewer::clearEditor() {
    mEditors.clear();
    mSelectedEditor.reset();
    update();
}

void ImageViewer::setInSelect(bool pixelSelect) {
    mInPixelSelect = pixelSelect;
}
