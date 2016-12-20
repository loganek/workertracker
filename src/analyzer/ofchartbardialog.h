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

class OfGroupPolicyWrapperBase
{
public:
    virtual ~OfGroupPolicyWrapperBase() {}
    virtual std::size_t get_size() const = 0;
    virtual std::pair<std::time_t, int> operator[](std::size_t index) const = 0;
    virtual QStringList get_x_description() const = 0;
    virtual QString get_title() const = 0;
};

template<typename GroupPolicy>
class OfGroupPolicyWrapper : public OfGroupPolicyWrapperBase
{
    typename GroupPolicy::container_t container;
public:
    OfGroupPolicyWrapper(const typename GroupPolicy::container_t& container) : container(container) {}
    std::size_t get_size() const override { return std::tuple_size<typename GroupPolicy::container_t>::value; }
    virtual std::pair<std::time_t, int> operator[](std::size_t index) const override { return container[index]; }
    QStringList get_x_description() const override;
    QString get_title() const override;
};

class OfChartBarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OfChartBarDialog(const OfGroupPolicyWrapperBase &policy_wrapper, QWidget *parent = 0);
    ~OfChartBarDialog();

private:
    void create_bar_series(const OfGroupPolicyWrapperBase &policy_wrapper);
    void create_info_tree(const OfGroupPolicyWrapperBase &policy_wrapper);
    void create_chart();
    void set_series(QT_CHARTS::BarSeries* series);

    Ui::OfChartBarDialog *ui;

    QStringList x_description;

    std::unordered_map<QT_CHARTS::BarSeries*, double> max_of_series;

    QT_CHARTS::BarSeries* total_series;
    QT_CHARTS::BarSeries* average_series;
    QT_CHARTS::BarSeries* current_series = nullptr;
    QT_CHARTS::ValueAxis* time_axis;
    QT_CHARTS::Chart* chart;
};

#endif // OFCHARTBARDIALOG_H
