/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef QTANALYZERWINDOW_H
#define QTANALYZERWINDOW_H

#include "imainwindow.h"

#include <QMainWindow>

#include <array>

class QTreeView;
class QLabel;
class QAbstractItemModel;
class QCheckBox;

namespace Ui {
class QtAnalyzerWindow;
}

class QtAnalyzerWindow : public QMainWindow, public IMainWindow
{
    Q_OBJECT

    QLabel *total_label;
    Ui::QtAnalyzerWindow *ui;
    std::array<QCheckBox*, 7> weekday_checkbox;

    enum PredefinedDateTime
    {
        TODAY = 0,
        YESTERDAY,
        THIS_MONTH,
        LAST_MONTH,
        CUSTOM
    };

    void init_secret();
    void connect_signals();
    void perform_search();
    void load_data_file();
    void set_current_range();
    void update_total_time(const std::chrono::seconds& seconds) override;

    void set_to_predefined_datetime(PredefinedDateTime type);

public:
    explicit QtAnalyzerWindow(QWidget *parent = 0);
    ~QtAnalyzerWindow();

    void set_model(QAbstractItemModel *model) override;
    void print_error(const std::string &) override;
    void update_for_new_model() override;
    void set_controller(AnalyzerController *controller) override;
    void show_window() override { this->show(); }

private slots:
    void on_exitAction_triggered(bool);
    void on_applyFilterButton_clicked(bool checked = true);
};

#endif // QTANALYZERWINDOW_H
