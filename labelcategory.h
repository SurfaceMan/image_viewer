#ifndef LABELCATEGORY_H
#define LABELCATEGORY_H

#include <QObject>
#include <QSharedPointer>

#include "types.h"

class Label;

class LabelCategory : public QObject {
    Q_OBJECT
public:
    explicit LabelCategory(QObject *parent = nullptr);

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(int lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged FINAL)
    Q_PROPERTY(
        QString description READ description WRITE setDescription NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visiableChanged FINAL)

    int     id() const;
    QString name() const;
    QColor  color() const;
    int     lineWidth() const;
    QString description() const;
    bool    visible() const;

    void setId(int id);
    void setName(QString name);
    void setColor(QColor color);
    void setLineWidth(int width);
    void setDescription(QString desc);
    void setVisible(bool visible);

signals:
    void idChanged();
    void nameChanged();
    void colorChanged();
    void lineWidthChanged();
    void descriptionChanged();
    void visiableChanged();

private:
    int     mId = 0;
    QString mName;
    QColor  mColor     = Qt::green;
    int     mLineWidth = 2;
    QString mDescription;
    bool    mVisible = true;
};

#endif // LABELCATEGORY_H
