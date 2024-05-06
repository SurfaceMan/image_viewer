#include "imagelabel.h"

ImageLabel::ImageLabel() = default;

void ImageLabel::onPaint(const PaintInfo &info) {
    info.painter->drawImage(0, 0, mImage);
}

void ImageLabel::setImage(const QImage &image) {
    mImage = image;
}

const QImage &ImageLabel::image() const {
    return mImage;
}
