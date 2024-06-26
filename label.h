#ifndef LABEL_H
#define LABEL_H

#include "labelcategory.h"

class Label {
public:
    Label();
    virtual ~Label() = default;

    void                          setCategory(QSharedPointer<LabelCategory> category);
    QSharedPointer<LabelCategory> category() const;

    virtual void        onPaint(const PaintInfo &info) = 0;
    virtual QStringList serialize() const;
    virtual void        deserialize(const QStringList &source);

private:
    QSharedPointer<LabelCategory> mCategory;
};

#endif // LABEL_H
