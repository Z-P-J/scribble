#include "sizedialog.h"
#include "ui_sizedialog.h"
#include <QDebug>
SizeDialog::SizeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SizeDialog)
{
    ui->setupUi(this);
    savesetting = false;
    width = 800;
    height = 800;
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(saveSetting()));

}
void SizeDialog::setWidget(ScribbleArea *y)
{
    s = y;
}
void SizeDialog::saveSetting()
{
   savesetting = true;
   width = ui->widthInput->value();
   height = ui->heightInput->value();
   qDebug() << 1;
   s->setSize(QSize(width, height));
   qDebug() << 2;
   this->close();
}

SizeDialog::~SizeDialog()
{
    delete ui;
}
