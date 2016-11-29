#include "smartpieseries.h"

QSmartPieSeries::QSmartPieSeries(std::shared_ptr<PieSeriesPolicy> policy, QObject *parent)
    : QPieSeries(parent),
      policy(policy)
{
}

void QSmartPieSeries::smart_add(QPieSlice *slice)
{
    if (policy->check_pie_slice(slice))
    {
        QPieSeries::operator <<(slice);
    }
    else
    {
        if (!others_slice)
        {
            others_slice = new DrilldownSlice(0, tr("Others"), -1);
        }
        others_slice->setValue(others_slice->value() + slice->value());
        delete slice;
    }
}

void QSmartPieSeries::finalize()
{
    if (others_slice)
    {
        operator <<(others_slice);
        others_slice = nullptr;
    }
}

bool MaxCountPieSeriesPolicy::check_pie_slice(QPieSlice *)
{
    current_count++;
    return current_count <= max_count;
}


bool MaxCountPieSeriesPolicy::need_update(const std::shared_ptr<PieSeriesPolicy> &other)
{
    auto policy = std::dynamic_pointer_cast<MaxCountPieSeriesPolicy>(other);
    return !policy || max_count != policy->max_count;
}

bool MinPercentagePieSeriesPolicy::check_pie_slice(QPieSlice *slice)
{
    return slice->value() / total_count >= min_percentage;
}

bool MinPercentagePieSeriesPolicy::need_update(const std::shared_ptr<PieSeriesPolicy> &other)
{
    auto policy = std::dynamic_pointer_cast<MinPercentagePieSeriesPolicy>(other);
    return !policy || min_percentage != policy->min_percentage || total_count != policy->total_count;
}
