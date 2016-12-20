/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "drilldownchart.h"
#include "smartpieseries.h"
#include "drilldownslice.h"

#include "wtcommon/datetimeutils.h"

#include <QPieSeries>

QT_CHARTS_USE_NAMESPACE

DrilldownChart::DrilldownChart(const std::shared_ptr<PieSeriesPolicy> &policy, const WT::ProcNameGroupPolicy::container_t& container, QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags),
      model(container),
      m_currentSeries(0),
      policy(policy)
{
    total_time = 0;

    for (const auto& proc : model)
    {
        for (const auto& desc : proc.second)
        {
            proc_duration[proc.first] += desc.second;
            total_time += desc.second;
        }
    }

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

QSmartPieSeries* DrilldownChart::get_series(const std::string &proc_name)
{
    qlonglong duration = proc_name.empty() ? total_time : proc_duration[proc_name];
    QVariant variant; variant.setValue(duration);

    std::unordered_map<std::string, std::time_t> the_model = proc_name.empty() ? proc_duration : model.at(proc_name);

    auto series = new QSmartPieSeries(policy->make_new(variant));

    series->setName(QString::fromStdString(proc_name));

    std::vector<DrilldownSlice*> slices;

    for (const auto &value : the_model)
    {
        if (value.second == 0)
        {
            continue;
        }

        if (!is_full)
        {
            slices.push_back(new DrilldownSlice(value.second, value.first, proc_name.empty()));
            continue;
        }
        for (const auto& detail_value: model.at(value.first))
        {
            if (detail_value.second == 0)
            {
                continue;
            }
            slices.push_back(new DrilldownSlice(detail_value.second, detail_value.first, false));
        }
    }

    std::sort(slices.begin(), slices.end(), [](DrilldownSlice* left, DrilldownSlice* right) { return left->value() > right->value(); });

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
    change_series(get_series(drilldownSlice->get_name()));
}

#include "moc_drilldownchart.cpp"
