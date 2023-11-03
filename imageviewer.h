#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QImage>
#include <QWidget>

#include "label.h"
#include "labeleditor.h"

class ImageViewer : public QWidget {
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = nullptr);

protected:
    // begin: qt widget events
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    // end: qt widget events

public slots:
    void zoomIn();
    void zoomOut();
    void fitToView();
    void resetToOriginalSize();

    void   loadImage(const QString &filepath);
    void   setImage(const QImage &img);
    QImage image() const;

    void setInSelect(bool pixelSelect);

    void addLabel(const QSharedPointer<Label> &label);
    void removeLabel(const QSharedPointer<Label> &label);
    void clearLabel();

    void addEditor(const QSharedPointer<LabelEditor> &editor);
    void removeEditor(const QSharedPointer<LabelEditor> &editor);
    void clearEditor();

signals:
    void scaleFactorChanged(double factor);
    void imageSizeChanged(const QSize &size);
    void pixelValueChanged(const QPoint &pos, const QColor &value);

private:
    void       resetWorldTransform();
    QTransform getWorldTransform() const;
    void       setWorldScale(double value);
    double     getWorldScale() const;
    void       setWorldScaleAndUpdate(double value);

    QPointF getMousePos();
    void    setMousePos(QMouseEvent *);

private:
    // file model
    const int INVALID_INDEX = -1;
    QImage    mImg;
    int       mSelectedEditorIndex = INVALID_INDEX;

    QList<QSharedPointer<LabelEditor>> mEditors;
    QList<QSharedPointer<Label>>       mLabels;

    // scale and transform of the objects on the desktop
    double       mWorldScale        = 1;
    const double mScaleFactor       = 1.1;
    QPointF      mWorldOffset       = {1, 1};
    QPointF      mImageOriginOffset = {-0.5, -0.5};
    const double mMinScale          = 0.01;
    const double mMaxScale          = 100.;
    const double mScaleStep         = 0.1;

    bool mFitToViewOnLoad   = true; // fit loaded image to view when it is loaded by SetBackground
    bool mFitToViewOnResize = true; // fit loaded image to view when widow is resized

    QPointF mMousePos;       // mouse position in background picture coordinates
    QPoint  mMousePosPixels; // mouse position in window coordinate system
    double  mMouseAngle = 0;

    // current mode
    bool mInPixelSelect = false;

    QImage mBackground;
};

#endif // IMAGEVIEWER_H
