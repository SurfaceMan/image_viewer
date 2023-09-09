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

    enum class TYPE : int { RECT, POLYGON };
    Q_ENUM(TYPE)

    Q_PROPERTY(int id READ getId WRITE setId NOTIFY idChanged FINAL);
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged FINAL);
    Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged FINAL);
    Q_PROPERTY(int lineWidth READ getLineWidth WRITE setLineWidth NOTIFY lineWidthChanged FINAL);
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY
                   descriptionChanged FINAL);
    Q_PROPERTY(TYPE type READ getType WRITE setType NOTIFY typeChanged FINAL);

    int     getId();
    QString getName();
    QColor  getColor();
    int     getLineWidth();
    QString getDescription();
    TYPE    getType();

    void setId(int id);
    void setName(QString name);
    void setColor(QColor color);
    void setLineWidth(int width);
    void setDescription(QString desc);
    void setType(TYPE type);

signals:
    void idChanged();
    void nameChanged();
    void colorChanged();
    void lineWidthChanged();
    void descriptionChanged();
    void typeChanged();

private:
    int     id;
    QString name;
    QColor  color;
    int     lineWidth;
    QString description;
    TYPE    type;

    QList<QSharedPointer<Label>> sharedLabels;
};

#endif // LABELCATEGORY_H
