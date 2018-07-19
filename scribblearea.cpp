
#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "scribblearea.h"
#include <QPainterPath>
#include <QFont>
#include <QLinearGradient>
#include <iterator>
#include <QDebug>
#include <QMatrix>
#include <QBrush>
#include <QGradient>
#include "scribbleshape.h"

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;

    filling = new int(0);
    fillbrush = new  QBrush(Qt::SolidPattern);

    myPenWidth = 1;
    myPenColor = Qt::blue;
    myPenType = Qt::SolidLine;
    type = ScribbleArea::Pen;
    adjustSize();
   // setMaximumSize(400,400);

    bgColor = qRgb(255, 255, 255);
    image = QImage(700, 500, QImage::Format_RGB32);
    image.fill(bgColor);
    tempImage = image;
    moveTime = 0;
    selected = false;


}

bool ScribbleArea::openImage(const QString &fileName)

{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

   // QSize newSize = loadedImage.size().expandedTo(size());
  // resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScribbleArea::set2bgColor()
{
    myPenColor = bgColor;
}
void ScribbleArea::setSize(QSize s)
{
    setMaximumSize(s.width(), s.height());
}
void ScribbleArea::setbgColor(const QColor &bgc)
{
    bgColor = bgc;
    image.fill(bgColor);
}
void ScribbleArea::setPenType(int s)
{
    myPenType = s;
}
void ScribbleArea::setShape(ScribbleArea::shape s)
{
    type = s;
}
void ScribbleArea::setMovement()
{

    type = Move;
    selected = true;

}
void ScribbleArea::setFillStyle(int *s, QBrush *brush)
{
     filling = s;
     fillbrush = brush;
}

void ScribbleArea::clearImage()

{
    image.fill(bgColor);
    modified = true;
    states.clear();
    moveStates.clear();
    update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if(type != Move){
        moveTime = 0;
    }
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();

    }
    if(event->button() == Qt::LeftButton && !shapeSet.empty() && filling)
    {
        foreach(ScribbleShape i, shapeSet)
        {
            if(i.shapetype == *filling && i.isInside(event->pos()))
            {
                QPainter painter(&image);
                painter.fillPath(i.path, *fillbrush);
                update();
                break;
            }
        }
    }

}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && !filling)
    {
        endPoint = event->pos();
        scribbling = true;
            tempImage = image;
            if(type == Pen)
                paint(image);
              //  drawLineTo(event->pos());
            else
                paint(tempImage);


    }

}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{

    scribbling = false;
    endPoint = event->pos();
    if(type != Pen && !selected && !filling)
        paint(image);

    if(type == Move)
        ++moveTime;


}


void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
        if(scribbling == true && type != Pen)
            painter.drawImage(0, 0, tempImage);
        else
            painter.drawImage(0, 0, image);

}



void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}



void ScribbleArea::paint(QImage &theImage)
{
    QPainter painter(&theImage);

    if(myPenColor == bgColor)
        myPenColor = Qt::blue;
    if(myPenType == Qt::SolidLine)
        state.pen = QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                                          Qt::RoundJoin);
    else if(myPenType == Qt::DotLine)
        state.pen = QPen(myPenColor, myPenWidth, Qt::DotLine, Qt::RoundCap,
                         Qt::RoundJoin);
    else if(myPenType == Qt::DashLine)
        state.pen = QPen(myPenColor, myPenWidth, Qt::DashLine, Qt::RoundCap,
                         Qt::RoundJoin);
    else if(myPenType == Qt::DashDotLine)
        state.pen = QPen(myPenColor, myPenWidth, Qt::DashDotLine, Qt::RoundCap,
                         Qt::RoundJoin);
    else
        state.pen = QPen(myPenColor, myPenWidth, Qt::DashDotDotLine, Qt::RoundCap,
                         Qt::RoundJoin);

    painter.setPen(state.pen);

    switch(type)
    {
        case ScribbleArea::Line:
        {
            painter.drawLine(lastPoint, endPoint);
            break;
        }
    case ScribbleArea::Rect:
    {
        QPainterPath path;
        QRect rect = QRect(lastPoint.x(), lastPoint.y(), endPoint.x(), endPoint.y());
        path.addRect(rect);
        painter.drawPath(path);
        if(scribbling == false)
        {
            QPoint v[4] = {rect.topLeft(), rect.bottomRight(), rect.bottomLeft(), rect.topRight()};
            ScribbleShape temp(ScribbleArea::Rect, v, path);
            shapeSet.push_back(temp);
        }
        if(scribbling == false)
        {
            state.path = path;
            states.push_back(state);
            state.path= state.path.subtracted(state.path);
        }
        break;
    }
    case ScribbleArea::Ellipse:
    {
         QPainterPath path;
         path.addEllipse(lastPoint.x(), lastPoint.y(), endPoint.x() - lastPoint.x(), endPoint.y() - lastPoint.y());
         painter.drawPath(path);
         if(scribbling == false)
         {
             QPoint v[4] = {lastPoint, endPoint, endPoint, lastPoint};
             ScribbleShape temp(ScribbleArea::Ellipse, v, path);
             shapeSet.push_back(temp);
         }
         if(scribbling == false)
         {
             state.path = path;
             states.push_back(state);
             state.path= state.path.subtracted(state.path);
         }
         break;
    }
    case ScribbleArea::Pen:
    {
        state.path.moveTo(lastPoint);
        state.path.lineTo(endPoint);
        painter.drawPath(state.path);
        states.push_back(state);
        state.path= state.path.subtracted(state.path);
        lastPoint = endPoint;
        break;
    }
    case ScribbleArea::Move:
    {
        if(moveTime == 0)
        {
            painter.setPen(Qt::DashLine);
            painter.drawRect(lastPoint.x(), lastPoint.y(), endPoint.x(), endPoint.y());
            pre = lastPoint;
            cur = endPoint;
        }
        if(moveTime > 1)
        {
            selected = false;
            for(QVector <State> :: iterator iter = states.begin();iter != states.end();){
                if((*iter).path.currentPosition().x()< cur.x() && (*iter).path.currentPosition().x() > pre.x() && (*iter).path.currentPosition().y() < cur.y() && (*iter).path.currentPosition().y() > pre.y())
                {
                    moveStates.push_back(*iter);
                    states.erase(iter);
                }else{
                    iter ++;
                }
            }
            for(int i = 0;i<moveStates.size();++i){
                moveStates[i].path.translate(endPoint.x()-lastPoint.x(),endPoint.y()-lastPoint.y());
            }

            lastPoint = endPoint;
            state.path= state.path.subtracted(state.path);

        }
        break;
    }
    }

    foreach (State state, states) {
        painter.setPen(state.pen);
        painter.drawPath(state.path);
    }
    foreach (State moveState, moveStates) {
        painter.setPen(moveState.pen);
        painter.drawPath(moveState.path);
    }
    state.path.moveTo(0,0);

    modified = true;
    update();
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void ScribbleArea::print()
{
#if QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_CONFIG(printdialog)
}

