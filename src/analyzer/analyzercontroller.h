/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef ANALYZERCONTROLLER_H
#define ANALYZERCONTROLLER_H

#include "imainwindow.h"
#include "qtfilterproxymodel.h"

#include "wtcommon/dataaccess.h"
#include "wtcommon/configuration.h"

#include <QStandardItemModel>
#include <QApplication>

#include <boost/optional.hpp>

#include <array>
#include <memory>

struct DataRange
{
    std::array<bool, 7> weekdays;
    std::time_t from_time = 0;
    std::time_t to_time = 0;
    int from_hour = -1;
    int to_hour = -1;
    DataRange(const std::array<bool, 7>& weekdays, std::time_t from_time,
              std::time_t to_time, int from_hour, int to_hour)
        : weekdays(weekdays), from_time(from_time), to_time(to_time),
          from_hour(from_hour), to_hour(to_hour)
    {}
    DataRange() {}
};

class AnalyzerController
{
    QApplication app;
    std::shared_ptr<WT::Configuration> config;
    QTFilterProxyModel proxy_model;
    DataRange data_range;

    boost::optional<std::regex> filter_pattern = boost::none;
    std::shared_ptr<IMainWindow> main_window;
    std::shared_ptr<WT::DataAccess> data_access;

    QList<QStandardItem*> create_model_item(const std::string &name, qlonglong time);
    void load_model(const WT::DataContainer &container);
    std::shared_ptr<WT::BinaryExpression> build_expression_from_range();

public:
    AnalyzerController(int argc, char **argv);
    virtual ~AnalyzerController() {}

    int run();
    void load_from_file(const std::string &filename);
    void set_range(const DataRange& data_range);
    void apply_filter(const std::string &search_text, bool case_sensitive);
    const QTFilterProxyModel& get_proxy_model() { return proxy_model; }
};

#endif // ANALYZERCONTROLLER_H
