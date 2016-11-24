/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
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

    void load_model(const WT::DataContainer &container) override;
    void apply_filter(const std::string &search_text, bool case_sensitive) override;
    IMainWindow* construct_window() override;

    static QList<QStandardItem*> create_model_item(const std::string &name, qlonglong time);

public:
    int run(int argc, char **argv) override;
};

#endif // QTANALYZERCONTROLLER_H
