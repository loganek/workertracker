/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include "drilldownchart.h"
#include <QDialog>

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
class QPieSlice;
QT_CHARTS_END_NAMESPACE

class QTFilterProxyModel;

namespace Ui {
class GraphDialog;
}

class GraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphDialog(QTFilterProxyModel *model, QWidget *parent = 0);

    ~GraphDialog();

private:
    Ui::GraphDialog *ui;
    DrilldownChart *chart;
    QTFilterProxyModel *model;

    void change_mode(bool v = true);

    void handleSliceClicked(QPieSlice *slice);
};

#endif // GRAPHDIALOG_H
