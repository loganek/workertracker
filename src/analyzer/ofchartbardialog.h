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

#include <QtCharts/QChartGlobal>
#include <QDialog>

QT_CHARTS_BEGIN_NAMESPACE
class QBarSet;
class QBarSeries;
class QValueAxis;
class QChart;
QT_CHARTS_END_NAMESPACE

namespace QT_CHARTS {
    QT_CHARTS_USE_NAMESPACE
    using BarSet = QBarSet;
    using BarSeries = QBarSeries;
    using ValueAxis = QValueAxis;
    using Chart = QChart;
}

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
    void create_bar_series(const WT::WeekdayGroupPolicy::container_t &container);
    void create_info_tree(const WT::WeekdayGroupPolicy::container_t &container);
    void create_chart();
    void set_series(QT_CHARTS::BarSeries* series);

    Ui::OfChartBarDialog *ui;

    QStringList weekdays;

    std::unordered_map<QT_CHARTS::BarSeries*, double> max_of_series;

    QT_CHARTS::BarSeries* total_series;
    QT_CHARTS::BarSeries* average_series;
    QT_CHARTS::BarSeries* current_series = nullptr;
    QT_CHARTS::ValueAxis* time_axis;
    QT_CHARTS::Chart* chart;
};

#endif // OFCHARTBARDIALOG_H
