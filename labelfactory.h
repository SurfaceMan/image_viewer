#ifndef LABELFACTORY_H
#define LABELFACTORY_H

#include "polygoneditor.h"
#include "recteditor.h"

struct LabelFactory {
    static QSharedPointer<Label> createLabel(LabelCategory::TYPE type, const QPointF &pos,
                                             const QSharedPointer<LabelCategory> &category) {
        QSharedPointer<Label> label;

        switch (type) {
            case LabelCategory::TYPE::RECT: {
                label.reset(new RectEditor());
                break;
            }
            case LabelCategory::TYPE::POLYGON: {
                label.reset(new PolygonEditor());
                break;
            }
            default:
                break;
        }

        if (label) {
            auto *editor = dynamic_cast<LabelEditor *>(label.get());
            editor->select(pos);
            label->setCategory(category);
        }

        return label;
    }
};

#endif // LABELFACTORY_H
