/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef DRILLDOWNCHART_H
#define DRILLDOWNCHART_H

#include "wtcommon/datacontainer.h"

#include <QtCharts/QChart>

#include <memory>

QT_CHARTS_BEGIN_NAMESPACE
class QPieSlice;
QT_CHARTS_END_NAMESPACE

class PieSeriesPolicy;
class QSmartPieSeries;

QT_CHARTS_USE_NAMESPACE

class DrilldownChart : public QChart
{
    Q_OBJECT
    WT::ProcNameGroupPolicy::container_t model;

    bool is_full = false;
    QSmartPieSeries *m_currentSeries = nullptr;
    std::shared_ptr<PieSeriesPolicy> policy;

    QSmartPieSeries* get_series(const std::string &proc_name = std::string());
    void remove_current_series();

    std::time_t total_time;
    std::unordered_map<std::string, std::time_t> proc_duration;

public:
    DrilldownChart(const std::shared_ptr<PieSeriesPolicy> &policy, const WT::ProcNameGroupPolicy::container_t& container, QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    ~DrilldownChart();
    void change_series(QSmartPieSeries *series);
    void set_model_type(bool is_full);
    void set_policy(const std::shared_ptr<PieSeriesPolicy>& policy);

public Q_SLOTS:
    void handleSliceClicked(QPieSlice *slice);
};

#endif // DRILLDOWNCHART_H
