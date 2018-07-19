#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QPainterPath>
#include <QVector>
#include <QPen>
#include "scribbleshape.h"

class ScribbleArea : public QWidget
{
    Q_OBJECT

struct State
{
      QPainterPath path;
      QPen pen;
};
public:
    ScribbleArea(QWidget *parent = 0);
     enum shape
    {
        Line = 1,
        Rect,
        Ellipse,
        Pen,
        Move
    };

    int *filling;
    QBrush *fillbrush;

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setbgColor(const QColor &bgc);
    void setSize(QSize s);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    void setPenType(int s);
    void set2bgColor();
    void setShape(ScribbleArea::shape s);
    void setMovement();
    void setFillStyle(int *s, QBrush *brush);
    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    QColor getbgColor() const {return bgColor;}
    int penWidth() const { return myPenWidth; }

public slots:
    void clearImage();
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo();
    void paint(QImage &theImage);
    void resizeImage(QImage *image, const QSize &newSize);

    bool modified;
    bool scribbling;
    bool selected;

    int moveTime;
    int myPenWidth;
    int myPenType;
    ScribbleArea::shape type;
    QColor bgColor;
    QColor myPenColor;
    QImage image;
    QImage tempImage;
    QPoint lastPoint;
    QPoint endPoint;
    QPainterPath ppath;
    QPoint pre;
    QPoint cur;

    State state;
    QVector<State> states;
    QVector<State> moveStates;
    QVector<ScribbleShape> shapeSet;

};


#endif
