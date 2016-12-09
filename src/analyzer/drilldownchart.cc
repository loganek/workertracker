/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "drilldownchart.h"
#include "drilldownslice.h"
#include "qtfilterproxymodel.h"
#include "smartpieseries.h"

#include "wtcommon/datetimeutils.h"

#include <QPieSeries>

QT_CHARTS_USE_NAMESPACE

DrilldownChart::DrilldownChart(const std::shared_ptr<PieSeriesPolicy> &policy, const QTFilterProxyModel *model, QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags),
      model(model),
      m_currentSeries(0),
      policy(policy)
{
    QSmartPieSeries *series = get_series();
    change_series(series);

    setTheme(QChart::ChartThemeBlueCerulean);
}

DrilldownChart::~DrilldownChart()
{
    remove_current_series();
}

void DrilldownChart::set_model_type(bool is_full)
{
    this->is_full = is_full;
    change_series(get_series());
}

void DrilldownChart::set_policy(const std::shared_ptr<PieSeriesPolicy>& policy)
{
    this->policy = policy;
    change_series(get_series());
}

QSmartPieSeries* DrilldownChart::get_series(const QModelIndex &parent_index)
{
    auto val = model->data(model->index(parent_index.row(), 1)).toString();

    qlonglong total_count = parent_index.isValid() ? model->data(model->index(parent_index.row(), 2)).toLongLong() : model->get_total_time().count();
    QVariant variant; variant.setValue(total_count);

    auto series = new QSmartPieSeries(policy->make_new(variant));

    series->setName(val);

    std::vector<DrilldownSlice*> slices;

    for (int row = 0; row < model->rowCount(parent_index); row++)
    {
        qlonglong value = model->data(model->index(row, 2, parent_index)).toLongLong();
        if (value == 0)
        {
            continue;
        }

        if (!is_full)
        {
            slices.push_back(new DrilldownSlice(value, model->data(model->index(row, 1, parent_index)).toString(), parent_index.isValid() ? -1 : row));
            continue;
        }
        auto idx = model->index(row, 0);
        for (int child_row = 0; child_row < model->rowCount(idx); child_row++)
        {
            value = model->data(model->index(child_row, 2, idx)).toLongLong();
            if (value == 0)
            {
                continue;
            }
            slices.push_back(new DrilldownSlice(value, model->data(model->index(child_row, 1, idx)).toString(), -1));
        }
    }

    if (is_full)
    {
        std::sort(slices.begin(), slices.end(), [](DrilldownSlice* left, DrilldownSlice* right) { return left->value() > right->value(); });
    }

    for (auto slice : slices)
    {
        series->smart_add(slice);
    }

    series->finalize();

    connect(series, &QPieSeries::clicked, this, &DrilldownChart::handleSliceClicked);

    return series;
}

void DrilldownChart::change_series(QSmartPieSeries *series)
{
    if (series == m_currentSeries)
    {
        return;
    }

    remove_current_series();

    m_currentSeries = series;
    addSeries(series);
    setTitle(QString("%1 %2")
             .arg(series->name())
             .arg(WT::time_to_display(std::chrono::seconds((qlonglong)series->sum())).c_str()));
}

void DrilldownChart::remove_current_series()
{
    if (m_currentSeries)
    {
        m_currentSeries->clear();
        removeSeries(m_currentSeries);
        delete m_currentSeries;
    }
}

void DrilldownChart::handleSliceClicked(QPieSlice *slice)
{
    if (is_full)
        return;

    DrilldownSlice *drilldownSlice = static_cast<DrilldownSlice *>(slice);
    auto index = drilldownSlice->get_row() == -1 ? QModelIndex() : model->index(drilldownSlice->get_row(), 0);
    change_series(get_series(index));
}

#include "moc_drilldownchart.cpp"
