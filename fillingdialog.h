#ifndef FILLINGDIALOG_H
#define FILLINGDIALOG_H

#include <QDialog>
#include <QBrush>

namespace Ui {
class FillingDialog;
}

class FillingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FillingDialog(QWidget *parent = 0);
    void linkWith(QBrush *b,int *t);
    ~FillingDialog();

private:
    Ui::FillingDialog *ui;
    int *type;
    bool opaque;
    QBrush *brush;

public slots:
    void fillRect();
    void fillEllispe();
    void trans();
    void setBrushStyle();
    void setColor();
    void saveSetting();
};

#endif // FILLINGDIALOG_H
