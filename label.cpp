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
