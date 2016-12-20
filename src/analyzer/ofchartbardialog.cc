/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "ofchartbardialog.h"
#include "ui_ofchartbardialog.h"

#include "wtcommon/datetimeutils.h"

#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

template<>
QStringList OfGroupPolicyWrapper<WT::WeekdayGroupPolicy>::get_x_description() const
{
    return {QObject::tr("Sun"), QObject::tr("Mon"), QObject::tr("Tue"), QObject::tr("Wed"), QObject::tr("Thu"), QObject::tr("Fri"), QObject::tr("Sat")};
}

template<>
QStringList OfGroupPolicyWrapper<WT::HourGroupPolicy>::get_x_description() const
{
    QStringList ret;
    for (int i = 0; i < 24; i++)
    {
        ret.push_back(QString::number(i));
    }
    return ret;
}

template<>
QString OfGroupPolicyWrapper<WT::WeekdayGroupPolicy>::get_title() const
{
    return QObject::tr("Day of Week");
}

template<>
QString OfGroupPolicyWrapper<WT::HourGroupPolicy>::get_title() const
{
    return QObject::tr("Hour of Day");
}

OfChartBarDialog::OfChartBarDialog(const OfGroupPolicyWrapperBase &policy_wrapper, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OfChartBarDialog)
{
    ui->setupUi(this);

    setWindowTitle(policy_wrapper.get_title());
    x_description = policy_wrapper.get_x_description();
    create_bar_series(policy_wrapper);
    create_info_tree(policy_wrapper);

    create_chart();

    ui->chartInfoTree->expandAll();

    set_series(total_series);

    connect(ui->totalRadioButton, &QRadioButton::toggled, this, [this] (bool) {
        set_series(ui->totalRadioButton->isChecked() ? total_series : average_series);
    });

    connect(ui->viewDetailsCheckBox, &QCheckBox::toggled, this, [this] (bool checked) {
        ui->chartInfoTree->setVisible(checked);
    });

    ui->viewDetailsCheckBox->setChecked(false);
}

void OfChartBarDialog::set_series(QBarSeries* series)
{
    if (current_series)
    {
        chart->removeSeries(current_series);
    }

    current_series = series;
    time_axis->setRange(0, max_of_series[current_series]);
    chart->addSeries(current_series);
}

void OfChartBarDialog::create_bar_series(const OfGroupPolicyWrapperBase &policy_wrapper)
{
    constexpr double SecondsInHour = 60 * 60;

    auto barset_total = new QBarSet(tr("Total"));
    auto barset_average = new QBarSet(tr("Average"));

    double max_total = 0.0, max_average = 0.0;

    for (std::size_t i = 0; i < policy_wrapper.get_size(); i++)
    {
        auto hours = policy_wrapper[i].first / SecondsInHour;
        max_total = std::max(hours, max_total);
        barset_total->append(hours);

        auto average = (policy_wrapper[i].second ? policy_wrapper[i].first / (double)policy_wrapper[i].second : 0) / SecondsInHour;
        max_average = std::max(average, max_average);
        barset_average->append(average);
    }

    total_series = new QBarSeries(this);
    total_series->append(barset_total);
    max_of_series[total_series] = max_total;

    average_series = new QBarSeries(this);
    average_series->append(barset_average);
    max_of_series[average_series] = max_average;
}

void OfChartBarDialog::create_info_tree(const OfGroupPolicyWrapperBase &policy_wrapper)
{
    for (std::size_t i = 0; i < policy_wrapper.get_size(); i++)
    {
        auto average = policy_wrapper[i].second ? policy_wrapper[i].first / (double)policy_wrapper[i].second : 0;

        auto parent_item = new QTreeWidgetItem(ui->chartInfoTree, {x_description[i]});
        new QTreeWidgetItem(parent_item, {tr("Total: "), QString::fromStdString(WT::time_to_display(std::chrono::seconds(policy_wrapper[i].first)))});
        new QTreeWidgetItem(parent_item, {tr("Average: "), QString::fromStdString(WT::time_to_display(std::chrono::seconds(int(average))))});
        new QTreeWidgetItem(parent_item, {tr("Count: "), QString::number(policy_wrapper[i].second)});
    }

    ui->chartInfoTree->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

void OfChartBarDialog::create_chart()
{
    chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(x_description);
    chart->addAxis(axis, Qt::AlignBottom);

    time_axis = new QValueAxis();
    chart->addAxis(time_axis, Qt::AlignLeft);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartFrame->layout()->addWidget(chartView);
}

OfChartBarDialog::~OfChartBarDialog()
{
    delete ui;
}
