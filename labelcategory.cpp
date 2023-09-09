#include "labelcategory.h"

LabelCategory::LabelCategory(QObject *parent)
    : QObject(parent) {}

int LabelCategory::getId() {
    return id;
}

QString LabelCategory::getName() {
    return name;
}

QColor LabelCategory::getColor() {
    return color;
}

int LabelCategory::getLineWidth() {
    return lineWidth;
}

QString LabelCategory::getDescription() {
    return description;
}

LabelCategory::TYPE LabelCategory::getType() {
    return type;
}

void LabelCategory::setId(int id_) {
    id = id_;
    emit idChanged();
}

void LabelCategory::setName(QString name_) {
    name = name_;
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
    description = desc_;
    emit descriptionChanged();
}

void LabelCategory::setType(TYPE type_) {
    type = type_;
    emit typeChanged();
}
