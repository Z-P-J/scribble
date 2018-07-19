#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QScrollArea>
#include <QToolBar>
class ScribbleArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void save();
    void setbgc();
    void setCanvasSize();
    void penColor();
    void penWidth();
    void penType();
    void customDraw();
    void rectDraw();
    void ellipseDraw();
    void lineDraw();
    void erase();
    void movement();
    void fill();
    void about();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);

    ScribbleArea *scribbleArea;
    QScrollArea *scrollArea;
    QMenu *saveAsMenu;
    QMenu *penStyleMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *shapeMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;
    QToolBar *toolbar;

    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *saveAct;
    QAction *exitAct;
    QAction *setCanvasSizeAct;
    QAction *penColorAct;
    QAction *penWidthAct;
    QList<QAction *> penStyleActs;
    QAction *eraseAct;
    QAction *printAct;
    QAction *setbgcAct;
    QAction *clearScreenAct;
    QAction *customDrawAct;
    QAction *rectDrawAct;
    QAction *ellipseDrawAct;
    QAction *lineDrawAct;
    QAction *movementAct;
    QAction *fillAct;
    QAction *aboutAct;
};


#endif
