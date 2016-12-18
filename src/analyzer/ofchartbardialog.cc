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

OfChartBarDialog::OfChartBarDialog(const WT::WeekdayGroupPolicy::container_t &container, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OfChartBarDialog)
{
    ui->setupUi(this);

    weekdays << tr("Sun") << tr("Mon") << tr("Tue") << tr("Wed") << tr("Thu") << tr("Fri") << tr("Sat");

    create_bar_series(container);
    create_info_tree(container);

    create_chart();

    ui->chartInfoTree->expandAll();

    set_series(total_series);

    connect(ui->totalRadioButton, &QRadioButton::toggled, this, [this] (bool) {
        set_series(ui->totalRadioButton->isChecked() ? total_series : average_series);
    });
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

void OfChartBarDialog::create_bar_series(const WT::WeekdayGroupPolicy::container_t &container)
{
    constexpr double SecondsInHour = 60 * 60;

    auto barset_total = new QBarSet(tr("Total"));
    auto barset_average = new QBarSet(tr("Average"));

    double max_total = 0.0, max_average = 0.0;

    for (std::size_t i = 0; i < container.size(); i++)
    {
        auto hours = container[i].first / SecondsInHour;
        max_total = std::max(hours, max_total);
        barset_total->append(hours);

        auto average = (container[i].second ? container[i].first / (double)container[i].second : 0) / SecondsInHour;
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

void OfChartBarDialog::create_info_tree(const WT::WeekdayGroupPolicy::container_t &container)
{
    for (std::size_t i = 0; i < container.size(); i++)
    {
        auto average = container[i].second ? container[i].first / (double)container[i].second : 0;

        auto parent_item = new QTreeWidgetItem(ui->chartInfoTree, {weekdays[i]});
        new QTreeWidgetItem(parent_item, {tr("Total: "), QString::fromStdString(WT::time_to_display(std::chrono::seconds(container[i].first)))});
        new QTreeWidgetItem(parent_item, {tr("Average: "), QString::fromStdString(WT::time_to_display(std::chrono::seconds(int(average))))});
        new QTreeWidgetItem(parent_item, {tr("Count: "), QString::number(container[i].second)});
    }

    ui->chartInfoTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
}

void OfChartBarDialog::create_chart()
{
    chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(weekdays);
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
