#pragma once

#include "label.h"

#include <QImage>

class ImageLabel : public Label {
public:
    ImageLabel();
    void onPaint(const PaintInfo &info) override;

    void          setImage(const QImage &image);
    const QImage &image() const;

private:
    QImage mImage;
};