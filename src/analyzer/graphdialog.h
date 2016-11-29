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
#include <QChartView>

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
    QChartView chartView;
    Ui::GraphDialog *ui;
    DrilldownChart *chart;

    void policy_updated(bool = true);
    bool is_full_mode() const;
    std::shared_ptr<PieSeriesPolicy> update_policy();
};

#endif // GRAPHDIALOG_H
