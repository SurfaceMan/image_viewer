#ifndef LABEL_H
#define LABEL_H

#include "labelcategory.h"

class Label {
public:
    Label()          = default;
    virtual ~Label() = default;

    void                          setCategory(QSharedPointer<LabelCategory> category);
    QSharedPointer<LabelCategory> getCategory() const;

    virtual void        onPaint(const PaintInfo &info) = 0;
    virtual QStringList serialize() const;
    virtual void        deserialize(const QStringList &strs);

    virtual bool select(const QPointF &pos);
    virtual void moving(const QPointF &curPos, const QPointF &lastPos);
    virtual void release();
    void         abortCreation();
    bool         isCreation() const;

    virtual void modify(const QPointF &pos);
    virtual void rotate(double angleDelta);

protected:
    QSharedPointer<LabelCategory> mCategory;

    bool mInCreation = true;
};

#endif // LABEL_H
