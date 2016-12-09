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

#include <memory>

class AnalyzerController
{
    WT::DateRange period;
    QApplication app;
    std::shared_ptr<WT::Configuration> config;
    QTFilterProxyModel proxy_model;

    boost::optional<std::regex> filter_pattern = boost::none;
    std::shared_ptr<IMainWindow> main_window;
    std::shared_ptr<WT::DataAccess> data_access;

    QList<QStandardItem*> create_model_item(const std::string &name, qlonglong time);
    void load_model(const WT::DataContainer &container);
    std::shared_ptr<WT::BinaryExpression> build_expression_from_period(WT::DateRange period);

public:
    AnalyzerController(int argc, char **argv);
    virtual ~AnalyzerController() {}

    int run();
    void load_from_file(const std::string &filename);
    void set_period(const WT::DateRange &period);
    void apply_filter(const std::string &search_text, bool case_sensitive);
    const QTFilterProxyModel& get_proxy_model() { return proxy_model; }
};

#endif // ANALYZERCONTROLLER_H
