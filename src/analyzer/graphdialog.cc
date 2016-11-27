/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "graphdialog.h"
#include "ui_graphdialog.h"

#include "drilldownslice.h"
#include "qtfilterproxymodel.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieLegendMarker>
#include <QtCharts/QPieSlice>

#include <array>

QT_CHARTS_USE_NAMESPACE

GraphDialog::GraphDialog(QTFilterProxyModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphDialog),
    model(model)
{
    ui->setupUi(this);

    chart = new DrilldownChart();
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->verticalLayout->addWidget(chartView);

    connect(ui->overviewModeRadioButton, &QRadioButton::clicked, this, &GraphDialog::change_mode);
    connect(ui->fullModeRadioButton, &QRadioButton::clicked, this, &GraphDialog::change_mode);

    change_mode();
}

GraphDialog::~GraphDialog()
{
    delete ui;
}

void GraphDialog::add_detailed_series(QPieSeries *series, int row)
{
    static std::array<Qt::GlobalColor, 8> colors = { Qt::red, Qt::yellow, Qt::cyan, Qt::black, Qt::magenta, Qt::blue, Qt::gray, Qt::green };
    QPieSeries *detailed_series = isFullMode() ? series : new QPieSeries(this);
    QColor color;

    if (isFullMode())
    {
        color = colors[row % colors.size()];
    }
    else
    {
        detailed_series->setName(model->data(model->index(row, 1)).toString());
        QObject::connect(detailed_series, &QPieSeries::clicked, this, &GraphDialog::handleSliceClicked);
    }

    int light = 100;
    auto parent_index = model->index(row, 0);
    for (int child_row = 0; child_row < model->rowCount(parent_index); child_row++)
    {
        qlonglong value = model->data(model->index(child_row, 2, parent_index)).toLongLong();
        if (!value)
        {
            continue;
        }
        auto slice = new DrilldownSlice(value, model->data(model->index(child_row, 1, parent_index)).toString(), series);
        if (isFullMode())
        {
            slice->setColor(color.lighter(200-light));
            light = std::max(20.0, light * 0.90);
        }
        *detailed_series << slice;
    }

    if (!isFullMode())
    {
        *series << new DrilldownSlice(detailed_series->sum(), model->data(model->index(row, 1)).toString(), detailed_series);
    }
}

void GraphDialog::change_mode(bool)
{
    QPieSeries *series = new QPieSeries(this);

    for (int r = 0; r < model->rowCount(); r++)
    {
        if (model->data(model->index(r, 2)).toLongLong() == 0)
        {
            continue;
        }

        add_detailed_series(series, r);
    }

    chart->changeSeries(series);

    if (!isFullMode())
    {
        QObject::connect(series, &QPieSeries::clicked, this, &GraphDialog::handleSliceClicked);
    }
}

bool GraphDialog::isFullMode() const
{
    return ui->fullModeRadioButton->isChecked();
}

void GraphDialog::handleSliceClicked(QPieSlice *slice)
{
    DrilldownSlice *drilldownSlice = static_cast<DrilldownSlice *>(slice);
    chart->changeSeries(drilldownSlice->drilldownSeries());
}
