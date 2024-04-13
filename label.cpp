#include "label.h"

Label::Label() {
    mCategory.reset(new LabelCategory);
}

void Label::setCategory(QSharedPointer<LabelCategory> category) {
    mCategory = category;
}

QSharedPointer<LabelCategory> Label::category() const {
    return mCategory;
}

QStringList Label::serialize() const {
    return {};
}

void Label::deserialize(const QStringList &source) {
    Q_UNUSED(source);
}
