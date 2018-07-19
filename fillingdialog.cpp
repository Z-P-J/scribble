#include "fillingdialog.h"
#include "scribblearea.h"
#include "ui_fillingdialog.h"
#include <QColor>
#include <QColorDialog>
FillingDialog::FillingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FillingDialog)
{
    ui->setupUi(this);
    opaque = true;
    ui->comboBox->addItem("SolidPattern");
    ui->comboBox->addItem("Dense2Pattern");
    ui->comboBox->addItem("Dense4Pattern");
    ui->comboBox->addItem("Dense6Pattern");
    ui->comboBox->addItem("HorPattern");
    ui->comboBox->addItem("VerPattern");
    ui->comboBox->addItem("CrossPattern");

    connect(ui->RectangleButton, SLOT(click()), this, SIGNAL(fillRect()));
    connect(ui->EllipseButton, SLOT(click()), this, SIGNAL(fillRect()));
    connect(ui->comboBox, SLOT(currentIndexChanged(int)), this, SIGNAL(setBrushStyle()));
    connect(ui->colorButton, SLOT(click()), this, SIGNAL(setColor()));
    connect(ui->okButton, SLOT(click()), this, SIGNAL(saveSetting()));
}
void FillingDialog::linkWith(QBrush *b, int *t)
{
    brush = b;
    type = t;
}

FillingDialog::~FillingDialog()
{
    delete ui;
}
void FillingDialog::fillRect()
{
    *type = ScribbleArea::Rect;
}

void FillingDialog::fillEllispe()
{
    *type = ScribbleArea::Ellipse;
}
void FillingDialog::setBrushStyle()
{
    int i = ui->comboBox->currentIndex();
    brush->setStyle(Qt::SolidPattern + i < 6? (i - 2) * 2 : (i + 3));
}
void FillingDialog::trans()
{
    opaque = false;
}

void FillingDialog::setColor()
{
    QColor newColor = QColorDialog::getColor(Qt::black);
    if (newColor.isValid())
        brush->setColor(newColor);
}
void FillingDialog::saveSetting()
{

}
