#include "labelcategory.h"

LabelCategory::LabelCategory(QObject *parent)
    : QObject(parent) {}

int LabelCategory::getId() const {
    return id;
}

QString LabelCategory::getName() const {
    return name;
}

QColor LabelCategory::getColor() const {
    return color;
}

int LabelCategory::getLineWidth() const {
    return lineWidth;
}

QString LabelCategory::getDescription() const {
    return description;
}

void LabelCategory::setId(int id_) {
    id = id_;
    emit idChanged();
}

void LabelCategory::setName(QString name_) {
    name = std::move(name_);
    emit nameChanged();
}

void LabelCategory::setColor(QColor color_) {
    color = color_;
    emit colorChanged();
}

void LabelCategory::setLineWidth(int width) {
    lineWidth = width;
    emit lineWidthChanged();
}

void LabelCategory::setDescription(QString desc_) {
    description = std::move(desc_);
    emit descriptionChanged();
}
