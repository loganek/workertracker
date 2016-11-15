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

static QList<QStandardItem *> prepareRow(const std::string &first,
                                         long int third)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(QString::fromStdString(first));
    rowItems << new QStandardItem(QString::fromStdString(AnalyzerController::time_to_display(std::chrono::seconds(third))));
    return rowItems;
}

void QtAnalyzerController::load_model_to_view(const WT::DataContainer &container)
{
    auto treeView = dynamic_cast<QtAnalyzerWindow*>(main_window)->get_tree_view();
    auto standardModel = new QStandardItemModel();
    QStandardItem *root_item = standardModel->invisibleRootItem();

    for (auto app : container.get_keys())
    {
        QList<QStandardItem *> app_row = prepareRow(app, 0);
        long int sum = 0;
        for (auto details : container.get_values(app))
        {
            auto tme = container.get_duration(app, details).count();
            QList<QStandardItem *> detail_row = prepareRow(details, tme);
            app_row.first()->appendRow(detail_row);

            sum += tme;
        }

        app_row[1] = new QStandardItem(QString::fromStdString(AnalyzerController::time_to_display(std::chrono::seconds(sum))));
        root_item->appendRow(app_row);
    }

    treeView->setModel(standardModel);
    treeView->expandAll();
    treeView->resizeColumnToContents(0);
}


void QtAnalyzerController::apply_filter()
{
}

QtAnalyzerController::RegistrarSingle<QtAnalyzerController> QtAnalyzerController::registrar;
