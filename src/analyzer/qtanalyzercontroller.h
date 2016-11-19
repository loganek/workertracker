#ifndef QTANALYZERCONTROLLER_H
#define QTANALYZERCONTROLLER_H

#include "analyzercontroller.h"
#include "qtfilterproxymodel.h"

#include <QApplication>

class QStandardItem;

class QtAnalyzerController : public AnalyzerController
{
    std::shared_ptr<QApplication> app;
    QTFilterProxyModel proxy_model;

    static RegistrarSingle<QtAnalyzerController> registrar;

    void load_model_to_view(const WT::DataContainer &container) override;
    void apply_filter() override;
    IMainWindow* construct_window() override;

    static QList<QStandardItem*> create_model_item(const std::string &name, qlonglong time);

public:
    int run(int argc, char **argv) override;
};

#endif // QTANALYZERCONTROLLER_H
