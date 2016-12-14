/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef OFCHARTBARDIALOG_H
#define OFCHARTBARDIALOG_H

#include "wtcommon/datacontainer.h"

#include <QDialog>

namespace Ui {
class OfChartBarDialog;
}

class OfChartBarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OfChartBarDialog(const WT::DataContainer &container, QWidget *parent = 0);
    ~OfChartBarDialog();

private:
    Ui::OfChartBarDialog *ui;
};

#endif // OFCHARTBARDIALOG_H
