#ifndef REGIONEDITOR_H
#define REGIONEDITOR_H

#include "labeleditor.h"

#include <QPixmap>

class RegionEditor : public LabelEditor {
public:
    RegionEditor();
    ~RegionEditor();

    void onPaint(const PaintInfo &info) override;

    bool select(const QPointF &pos) override;
    void moving(const QPointF &curPos, const QPointF &lastPos) override;
    void release() override;
    void modify(const QPointF &pos) override;
    void rotate(double angleDelta) override;

    QVector<QPointF> region() const;
    void             setReion(const QVector<QPointF> &value);

    enum Tool { PEN, ERASER };
    enum Shape { RECT, CIRCLE };

    Tool tool() const;
    void setTool(Tool tool);

    double toolRadius() const;
    void   setToolRadius(double radius);

    Shape toolShape() const;
    void  setToolShape(Shape shape);

private:
    QVector<QPointF> mRegion;

    QPixmap                  mRenderedRegion;
    QSharedPointer<QPainter> mPainter;

    Tool    mTool       = PEN;
    Shape   mToolShape  = CIRCLE;
    double  mToolRadius = 12.;
    QPointF mCenter;

    bool mPressed = false;
};

#endif // REGIONEDITOR_H