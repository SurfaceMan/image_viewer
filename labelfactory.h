#ifndef LABELFACTORY_H
#define LABELFACTORY_H

#include "labelpolygon.h"
#include "labelrect.h"

struct LabelFactory {
    static QSharedPointer<Label> createLabel(LabelCategory::TYPE type, const QPointF &pos,
                                             const QSharedPointer<LabelCategory> &category) {
        QSharedPointer<Label> label;

        switch (type) {
            case LabelCategory::TYPE::RECT: {
                label.reset(new LabelRect());
                break;
            }
            case LabelCategory::TYPE::POLYGON: {
                label.reset(new LabelPolygon());
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
