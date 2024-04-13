#include "labelcategory.h"

LabelCategory::LabelCategory(QObject *parent)
    : QObject(parent) {}

int LabelCategory::id() const {
    return mId;
}

QString LabelCategory::name() const {
    return mName;
}

QColor LabelCategory::color() const {
    return mColor;
}

int LabelCategory::lineWidth() const {
    return mLineWidth;
}

QString LabelCategory::description() const {
    return mDescription;
}

bool LabelCategory::visible() const {
    return mVisible;
}

void LabelCategory::setId(int id_) {
    mId = id_;
    emit idChanged();
}

void LabelCategory::setName(QString name_) {
    mName = std::move(name_);
    emit nameChanged();
}

void LabelCategory::setColor(QColor color_) {
    mColor = color_;
    emit colorChanged();
}

void LabelCategory::setLineWidth(int width) {
    mLineWidth = width;
    emit lineWidthChanged();
}

void LabelCategory::setDescription(QString desc_) {
    mDescription = std::move(desc_);
    emit descriptionChanged();
}

void LabelCategory::setVisible(bool visible) {
    mVisible = visible;
    emit visiableChanged();
}
