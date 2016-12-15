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

#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

OfChartBarDialog::OfChartBarDialog(const WT::DataContainer &container, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OfChartBarDialog)
{
    ui->setupUi(this);

    auto data = container.get_grouped<WT::WeekdayGroupPolicy>();

    QBarSet *barset = new QBarSet("");
    constexpr double SecondsInHour = 60 * 60;
    for (std::size_t i = 0; i < data.size(); i++)
    {
        barset->append(data[i].first / SecondsInHour);
    }

    QBarSeries *series = new QBarSeries();
    series->append(barset);


    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << tr("Sun") << tr("Mon") << tr("Tue") << tr("Wed") << tr("Thu") << tr("Fri") << tr("Sat");
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartFrame->layout()->addWidget(chartView);
}

OfChartBarDialog::~OfChartBarDialog()
{
    delete ui;
}
