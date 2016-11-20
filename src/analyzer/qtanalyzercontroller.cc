#include "qtanalyzercontroller.h"
#include "qtanalyzerwindow.h"

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

    auto time_string = AnalyzerController::time_to_display(std::chrono::seconds(time));
    rowItems << new QStandardItem(QString::fromStdString(time_string));

    rowItems << new QStandardItem(QString::fromStdString(name));

    auto item = new QStandardItem();
    item->setData(QVariant(time), Qt::DisplayRole);
    rowItems << item;

    return rowItems;
}

void QtAnalyzerController::load_model_to_view(const WT::DataContainer &container)
{
    auto standard_model = new QStandardItemModel();
    QStandardItem *root_item = standard_model->invisibleRootItem();

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

        app_row[0]->setData(QString::fromStdString(AnalyzerController::time_to_display(std::chrono::seconds(total_time))), Qt::DisplayRole);
        app_row[2]->setData(total_time, Qt::DisplayRole);
        root_item->appendRow(app_row);
    }

    proxy_model.setSourceModel(standard_model);

    auto tree_view = dynamic_cast<QtAnalyzerWindow*>(main_window)->get_tree_view();
    tree_view->setModel(&proxy_model);

    main_window->update_for_new_model();
}

void QtAnalyzerController::apply_filter()
{
    proxy_model.setFilterRegExp(filter_pattern);
}

QtAnalyzerController::RegistrarSingle<QtAnalyzerController> QtAnalyzerController::registrar;
