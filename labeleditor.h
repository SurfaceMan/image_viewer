#ifndef LABELEDITOR_H
#define LABELEDITOR_H

#include "label.h"

class LabelEditor : public Label {
public:
    LabelEditor()          = default;
    virtual ~LabelEditor() = default;

    virtual bool select(const QPointF &pos);
    virtual void moving(const QPointF &curPos, const QPointF &lastPos);
    virtual void release();
    virtual void modify(const QPointF &pos);
    virtual void rotate(double angleDelta);

    void abortCreation();
    bool isCreation() const;

private:
    bool mInCreation = true;
};

#endif // LABELEDITOR_H