#include "labeleditor.h"

bool LabelEditor::select(const QPointF &pos) {
    Q_UNUSED(pos)
    return false;
}

void LabelEditor::moving(const QPointF &curPos, const QPointF &lastPos) {}

void LabelEditor::release() {}

void LabelEditor::abortCreation() {
    mInCreation = false;
}

bool LabelEditor::isCreation() const {
    return mInCreation;
}

void LabelEditor::modify(const QPointF &pos) {}

void LabelEditor::rotate(double angleDelta) {}