/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "analyzercontroller.h"

#include "wtcommon/sqlitedataaccess.h"
#include "wtcommon/logger.h"
#include "wtcommon/configuration.h"
#include "wtcommon/datetimeutils.h"

#include "qtanalyzerwindow.h"

#include <boost/date_time.hpp>

AnalyzerController::AnalyzerController(int argc, char **argv)
    : app(argc, argv),
      config(std::make_shared<WT::Configuration>())
{
}

int AnalyzerController::run()
{
    main_window = std::make_shared<QtAnalyzerWindow>();
    main_window->set_controller(this);

    load_from_file(config->get_general_param("data-path").get());

    main_window->show_window();

    return app.exec();
}

void AnalyzerController::apply_filter(const std::string &search_text, bool case_sensitive)
{
    try
    {
        std::regex::flag_type flags = std::regex::ECMAScript;
        if (!case_sensitive)
        {
            flags |= std::regex::icase;
        }
        filter_pattern = search_text.empty() ? boost::none : boost::make_optional(std::regex(search_text, flags));
    }
    catch (const std::regex_error &ex)
    {
        WT_LOG_W << ex.what();
        filter_pattern = boost::none;
    }

    proxy_model.setFilterRegExp(filter_pattern);
    main_window->update_total_time(proxy_model.get_total_time());
}

void AnalyzerController::load_from_file(const std::string &filename)
{
    try
    {
        config->set_general_param("data-path", filename);
        data_access = std::make_shared<WT::SQLiteDataAccess>(config);
        data_access->open(true);
        load_model(data_access->get_tree(period));
    }
    catch (const std::runtime_error &ex)
    {
        main_window->print_error(std::string("Error: ") + ex.what());
    }
}

void AnalyzerController::set_period(const WT::DateRange &period)
{
    this->period = period;
    if (data_access)
    {
        load_model(data_access->get_tree(period));
    }
}

QList<QStandardItem*> AnalyzerController::create_model_item(const std::string &name, qlonglong time)
{
    QList<QStandardItem *> rowItems;

    rowItems << new QStandardItem(QString::fromStdString(WT::time_to_display(std::chrono::seconds(time))));
    rowItems << new QStandardItem(QString::fromStdString(name));

    auto item = new QStandardItem();
    item->setData(QVariant(time), Qt::DisplayRole);
    rowItems << item;

    return rowItems;
}

void AnalyzerController::load_model(const WT::DataContainer &container)
{
    auto standard_model = proxy_model.get_source_model();
    standard_model->clear();

    for (auto app : container.get_keys())
    {
        QList<QStandardItem *> app_row = create_model_item(app, 0);
        qlonglong total_time = 0;
        for (auto details : container.get_values(app))
        {
            auto duration = container.get_duration(app, details).count();
            app_row.first()->appendRow(create_model_item(details, duration));

            total_time += duration;
        }

        app_row[0]->setData(QString::fromStdString(WT::time_to_display(std::chrono::seconds(total_time))), Qt::DisplayRole);
        app_row[2]->setData(total_time, Qt::DisplayRole);
        standard_model->invisibleRootItem()->appendRow(app_row);
    }

    main_window->set_model(&proxy_model);

    main_window->update_for_new_model();
    proxy_model.setFilterRegExp(filter_pattern);
    main_window->update_total_time(proxy_model.get_total_time());
}
