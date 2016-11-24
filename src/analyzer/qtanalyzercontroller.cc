/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "qtanalyzercontroller.h"
#include "qtanalyzerwindow.h"
#include "wtcommon/datetimeutils.h"

#include <QTreeView>
#include <QStandardItem>

int QtAnalyzerController::run(int argc, char **argv)
{
    app = std::make_shared<QApplication>(argc, argv);

    int ret = AnalyzerController::run(argc, argv);

    if (!ret)
    {
        dynamic_cast<QtAnalyzerWindow*>(main_window)->show();
        ret = app->exec();
        app.reset();
    }

    return ret;
}

IMainWindow* QtAnalyzerController::construct_window()
{
    return new QtAnalyzerWindow();
}

QList<QStandardItem*> QtAnalyzerController::create_model_item(const std::string &name, qlonglong time)
{
    QList<QStandardItem *> rowItems;

    rowItems << new QStandardItem(QString::fromStdString(WT::time_to_display(std::chrono::seconds(time))));
    rowItems << new QStandardItem(QString::fromStdString(name));

    auto item = new QStandardItem();
    item->setData(QVariant(time), Qt::DisplayRole);
    rowItems << item;

    return rowItems;
}

void QtAnalyzerController::load_model(const WT::DataContainer &container)
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

    dynamic_cast<QtAnalyzerWindow*>(main_window)->set_model(&proxy_model);

    main_window->update_for_new_model();
    proxy_model.setFilterRegExp(filter_pattern);
    main_window->update_total_time(proxy_model.get_total_time());
}

void QtAnalyzerController::apply_filter(const std::string &search_text, bool case_sensitive)
{
    AnalyzerController::apply_filter(search_text, case_sensitive);
    proxy_model.setFilterRegExp(filter_pattern);
    main_window->update_total_time(proxy_model.get_total_time());
}

QtAnalyzerController::RegistrarSingle<QtAnalyzerController> QtAnalyzerController::registrar;
