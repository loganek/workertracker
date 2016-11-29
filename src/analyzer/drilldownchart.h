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

#include <QtCharts/QChart>
#include <QModelIndex>

#include <memory>

QT_CHARTS_BEGIN_NAMESPACE
class QPieSlice;
QT_CHARTS_END_NAMESPACE

#include "smartpieseries.h"

class QTFilterProxyModel;
class QSmartPieSeries;

QT_CHARTS_USE_NAMESPACE

class DrilldownChart : public QChart
{
    Q_OBJECT
    QTFilterProxyModel *model;

    std::map<QString, QSmartPieSeries*> cache;
    QSmartPieSeries* cache_root = nullptr;
    QSmartPieSeries* cache_root_full = nullptr;
    bool is_full = false;
    QSmartPieSeries *m_currentSeries;
    std::shared_ptr<PieSeriesPolicy> policy;

    QSmartPieSeries* get_series(const QModelIndex &parent_index = QModelIndex());

public:
    explicit DrilldownChart(const std::shared_ptr<PieSeriesPolicy> &policy, QTFilterProxyModel *model, QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    ~DrilldownChart();
    void change_series(QSmartPieSeries *series);
    void set_model_type(bool is_full);
    void set_policy(const std::shared_ptr<PieSeriesPolicy>& policy);

public Q_SLOTS:
    void handleSliceClicked(QPieSlice *slice);
};

#endif // DRILLDOWNCHART_H
