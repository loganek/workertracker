#ifndef QTANALYZERCONTROLLER_H
#define QTANALYZERCONTROLLER_H

#include "analyzercontroller.h"

#include <QApplication>

class QtAnalyzerController : public AnalyzerController
{
    std::shared_ptr<QApplication> app;

    static RegistrarSingle<QtAnalyzerController> registrar;

    void load_model_to_view(const WT::DataContainer &container) override;
    void apply_filter() override;
    IMainWindow* construct_window() override;

public:
    int run(int argc, char **argv) override;
};

#endif // QTANALYZERCONTROLLER_H
