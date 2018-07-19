#include <QtWidgets>
#include <QtDebug>
#include "mainwindow.h"
#include "scribblearea.h"
#include "sizedialog.h"
#include <QScrollArea>
#include <fillingdialog.h>
#include <QToolBar>
#include <QDebug>

MainWindow::MainWindow()
{
   // scrollArea->setBackgroundRole(QPalette::Dark);
   // scrollArea->setWidget(scribbleArea);
   setWindowTitle("Scribble");
   setWindowIcon(QIcon(":/icon/1.png"));
   // setWindowOpacity(0.94);
   // setCentralWidget(scrollArea);

    scribbleArea = new ScribbleArea;
  //  setCentralWidget(scribbleArea);

   // scrollArea->show();

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(scribbleArea);
    scrollArea->setWidgetResizable(true);
    scrollArea->widget()->setMinimumSize(scribbleArea->width(), scribbleArea->height());

    setCentralWidget(scrollArea);

    createActions();
    createMenus();
    createToolBars();

    setWindowTitle(tr("Scribble"));
    resize(1500, 800);



}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}
void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
            scribbleArea->openImage(fileName);
    }
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::setCanvasSize()
{
    SizeDialog *dlg = new SizeDialog;
    dlg->setWindowTitle("SetCanvasSize");
    dlg->setWindowIcon(QIcon(":/icon/3.png"));
    dlg->setWidget(scribbleArea);
    dlg->show();

}
void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if (newColor.isValid())
        scribbleArea->setPenColor(newColor);
}

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width:"),
                                        scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}

void MainWindow::penType()
{
     QAction *action = qobject_cast<QAction *>(sender());
     int s = action->data().toInt();
     scribbleArea->setPenType(s);
}
void MainWindow::setbgc()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->getbgColor());
    if (newColor.isValid())
        scribbleArea->setbgColor(newColor);
}
void MainWindow::erase()
{
    scribbleArea->setShape(ScribbleArea::Pen);
    scribbleArea->setPenType(Qt::SolidLine);
    scribbleArea->set2bgColor();
}
void MainWindow::customDraw()
{
    scribbleArea->setShape(ScribbleArea::Pen);
}
void MainWindow::rectDraw()
{
    scribbleArea->setShape(ScribbleArea::Rect);
}
void MainWindow::ellipseDraw()
{
    scribbleArea->setShape(ScribbleArea::Ellipse);
}
void MainWindow::lineDraw()
{
    scribbleArea->setShape(ScribbleArea::Line);
}
void MainWindow::movement()
{
    scribbleArea->setMovement();
}
void MainWindow::fill()
{
    FillingDialog *fd = new FillingDialog;
    fd->setWindowIcon(QIcon(":/icon/2.png"));
    fd->setWindowTitle("SetFillingPattern");
    fd->linkWith(scribbleArea->fillbrush, scribbleArea->filling);
    fd->show();


}
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Scribble"),
            tr("<p>The <b>Scribble</b> example shows how to use QMainWindow as the "
               "base widget for an application, and how to reimplement some of "
               "QWidget's event handlers to receive the events generated for "
               "the application's widgets:</p><p> We reimplement the mouse event "
               "handlers to facilitate drawing, the paint event handler to "
               "update the application and the resize event handler to optimize "
               "the application's appearance. In addition we reimplement the "
               "close event handler to intercept the close events before "
               "terminating the application.</p><p> The example also demonstrates "
               "how to use QPainter to draw an image in real time, as well as "
               "to repaint widgets.</p>"));
}

void MainWindow::createActions()

{
    openAct = new QAction(QIcon(":/icon/open.png"),tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/icon/save.png"),tr("Save"), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(save()));

    setCanvasSizeAct = new QAction(QIcon(":/icon/save.png"),tr("Set canvas size"), this);
    connect(setCanvasSizeAct, SIGNAL(triggered()), this, SLOT(setCanvasSize()));

    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }

    QString penstyles[5] = {tr("&Solid&Line"), tr("&Dot&Line"), tr("&Dash&Line"), tr("&DashDot&Line"), tr("&DashDotDot&Line")};
    QIcon penicons[5] = {QIcon(":/icon/line.png"),QIcon(":/icon/dotline.png"),  QIcon(":/icon/dashline.png"),QIcon(":/icon/dashdotline.png"),QIcon(":/icon/dashdotdot.png")};
    int pentype[5] = {Qt::SolidLine, Qt::DotLine, Qt::DashLine, Qt::DashDotLine, Qt::DashDotDotLine};
    for(int p = 0; p < 5; p ++)
    {
        QAction *action = new QAction(penicons[p], penstyles[p], this);
        action->setData(pentype[p]);
        connect(action,SIGNAL(triggered()), this, SLOT(penType()));
        penStyleActs.append(action);
    }


    printAct = new QAction(QIcon(":/icon/print.png"),tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), scribbleArea, SLOT(print()));

    exitAct = new QAction(QIcon(":/icon/exit.png"),tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(QIcon(":/icon/pencolor.png"),tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    eraseAct = new QAction(QIcon(":/icon/eraser.png"),tr("&Eraser"), this);
    connect(eraseAct, SIGNAL(triggered()), this, SLOT(erase()));

    penWidthAct = new QAction(QIcon(":/icon/width.png"),tr("Pen &Width / Eraser &Width"), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    setbgcAct = new QAction(tr("Set the color of canvas"), this);
    connect(setbgcAct, SIGNAL(triggered()), this, SLOT(setbgc()));

    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()),
            scribbleArea, SLOT(clearImage()));

    customDrawAct = new QAction(tr("&Use pen"), this);
    connect(customDrawAct, SIGNAL(triggered()), this, SLOT(customDraw()));

    rectDrawAct = new QAction(QIcon(":/icon/rect.png"),tr("&Draw rectangle"), this);
    connect(rectDrawAct, SIGNAL(triggered()), this, SLOT(rectDraw()));

    ellipseDrawAct = new QAction(QIcon(":/icon/ellipse.png"),tr("&Draw ellipse"), this);
    connect(ellipseDrawAct, SIGNAL(triggered()), this, SLOT(ellipseDraw()));

    lineDrawAct = new QAction(QIcon(":/icon/line.png"),tr("&Draw line"), this);
    connect(lineDrawAct, SIGNAL(triggered()), this, SLOT(lineDraw()));

    movementAct = new QAction(QIcon(":/icon/move.png"),tr("&Move the selected area"), this);
    connect(movementAct, SIGNAL(triggered()), this, SLOT(movement()));

    fillAct = new QAction(QIcon(":/icon/fill.png"),tr("&Fill the shape"), this);
    connect(fillAct, SIGNAL(triggered()), this, SLOT(fill()));

    aboutAct = new QAction(QIcon(":/icon/about.png"),tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

}

void MainWindow::createMenus()
{
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);

    penStyleMenu = new QMenu(tr("&Set pen style"), this);
    foreach(QAction *action, penStyleActs)
        penStyleMenu->addAction(action);

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Pen"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addMenu(penStyleMenu);
    optionMenu->addSeparator();
    optionMenu->addAction(eraseAct);
    optionMenu->addAction(setbgcAct);
    optionMenu->addAction(setCanvasSizeAct);
    optionMenu->addAction(clearScreenAct);

    shapeMenu = new QMenu(tr("&Shape"), this);
    shapeMenu->addAction(customDrawAct);
    shapeMenu->addAction(rectDrawAct);
    shapeMenu->addAction(ellipseDrawAct);
    shapeMenu->addAction(lineDrawAct);

    toolsMenu = new QMenu(tr("&Tools"), this);
    toolsMenu->addAction(movementAct);
    toolsMenu->addAction(fillAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(shapeMenu);
    menuBar()->addMenu(toolsMenu);
    menuBar()->addMenu(helpMenu);


}

void MainWindow::createToolBars()
{
    toolbar = new QToolBar(this);
    toolbar->addAction(openAct);
    toolbar->addAction(saveAct);
    toolbar->addAction(exitAct);
    toolbar->addSeparator();

    toolbar->addActions(penStyleActs);
    toolbar->addAction(penWidthAct);
    toolbar->addAction(penColorAct);
    toolbar->addSeparator();

    toolbar->addAction(eraseAct);
    toolbar->addAction(clearScreenAct);
    toolbar->addSeparator();

    toolbar->addAction(customDrawAct);
    toolbar->addAction(rectDrawAct);
    toolbar->addAction(ellipseDrawAct);
    toolbar->addAction(lineDrawAct);
    toolbar->addSeparator();

    toolbar->addAction(customDrawAct);
    toolbar->addAction(rectDrawAct);
    toolbar->addAction(lineDrawAct);
    toolbar->addSeparator();

    toolbar->addAction(movementAct);
    toolbar->addAction(fillAct);
    toolbar->addSeparator();

    toolbar->addAction(aboutAct);

    this->addToolBar(toolbar);
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
}
bool MainWindow::maybeSave()
{
    if (scribbleArea->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Scribble"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveFile("png");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    } else {
        return scribbleArea->saveImage(fileName, fileFormat.constData());
    }
}
