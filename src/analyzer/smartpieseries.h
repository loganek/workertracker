#ifndef SMARTPIESERIES_H
#define SMARTPIESERIES_H

#include "drilldownslice.h"

#include <QPieSeries>

#include <memory>
#include <map>

QT_CHARTS_USE_NAMESPACE

class PieSeriesPolicy
{
public:
    virtual bool check_pie_slice(QPieSlice *slice) = 0;
    virtual std::shared_ptr<PieSeriesPolicy> make_new(QVariant variant = QVariant()) = 0;
    virtual bool need_update(const std::shared_ptr<PieSeriesPolicy> &other) = 0;
    virtual ~PieSeriesPolicy() {}
};

class AllPieSeriesPolicy : public PieSeriesPolicy
{
public:
    bool check_pie_slice(QPieSlice *) { return true; }
    std::shared_ptr<PieSeriesPolicy> make_new(QVariant) override { return std::make_shared<AllPieSeriesPolicy>(); }
    virtual bool need_update(const std::shared_ptr<PieSeriesPolicy> &other) override { return !std::dynamic_pointer_cast<AllPieSeriesPolicy>(other); }
};

class MaxCountPieSeriesPolicy : public PieSeriesPolicy
{
    const int max_count;
    int current_count = 0;

public:
    MaxCountPieSeriesPolicy(int max_count) : max_count(max_count) {}

    bool check_pie_slice(QPieSlice *) override;
    std::shared_ptr<PieSeriesPolicy> make_new(QVariant) override { return std::make_shared<MaxCountPieSeriesPolicy>(max_count); }
    bool need_update(const std::shared_ptr<PieSeriesPolicy> &other) override;
};

class MinPercentagePieSeriesPolicy : public PieSeriesPolicy
{
    const double min_percentage;
    const int total_count;

public:
    MinPercentagePieSeriesPolicy(double min_percentage, int total_count)
        : min_percentage(min_percentage), total_count(total_count) {}
    bool check_pie_slice(QPieSlice *slice) override;
    std::shared_ptr<PieSeriesPolicy> make_new(QVariant variant) override { return std::make_shared<MinPercentagePieSeriesPolicy>(min_percentage, variant.toLongLong()); }
    bool need_update(const std::shared_ptr<PieSeriesPolicy> &other) override;
};

class QSmartPieSeries : public QPieSeries
{
    std::shared_ptr<PieSeriesPolicy> policy;
    QPieSlice* others_slice = nullptr;

public:
    virtual ~QSmartPieSeries() {}

    explicit QSmartPieSeries(std::shared_ptr<PieSeriesPolicy> policy, QObject *parent = Q_NULLPTR);
    void smart_add(QPieSlice *slice);
    void finalize();
    std::shared_ptr<PieSeriesPolicy> get_policy() const { return policy; }
};


#endif // SMARTPIESERIES_H
