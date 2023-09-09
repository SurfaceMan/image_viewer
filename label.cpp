#include "label.h"

void Label::setCategory(QSharedPointer<LabelCategory> category) {
    mCategory = category;
}

QSharedPointer<LabelCategory> Label::getCategory() const {
    return mCategory;
}

QStringList Label::serialize() const {
    return {};
}

void Label::deserialize(const QStringList &strs) {
    Q_UNUSED(strs);
}

bool Label::select(const QPointF &pos) {
    Q_UNUSED(pos)
    return false;
}

void Label::moving(const QPointF &curPos, const QPointF &lastPos) {}

void Label::release() {}

void Label::abortCreation() {
    mInCreation = false;
}

bool Label::isCreation() {
    return mInCreation;
}

void Label::modify(const QPointF &pos) {}

void Label::rotate(double angleDelta) {}
