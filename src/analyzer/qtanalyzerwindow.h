#ifndef QTANALYZERWINDOW_H
#define QTANALYZERWINDOW_H

#include "imainwindow.h"

#include <QMainWindow>

class QTreeView;
class QLabel;
class QAbstractItemModel;

namespace Ui {
class QtAnalyzerWindow;
}

class QtAnalyzerWindow : public QMainWindow, public IMainWindow
{
    Q_OBJECT

    QLabel *total_label;
    Ui::QtAnalyzerWindow *ui;

    enum PredefinedDateTime
    {
        TODAY = 0,
        YESTERDAY,
        THIS_MONTH,
        LAST_MONTH,
        CUSTOM
    };

    void connect_signals();
    void perform_search();
    void load_data_file();
    void update_total_time(const std::chrono::seconds& seconds) override;

    bool set_datetime_lock;
    void set_period();
    void set_to_predefined_datetime(PredefinedDateTime type);

public:
    explicit QtAnalyzerWindow(QWidget *parent = 0);
    ~QtAnalyzerWindow();

    void set_model(QAbstractItemModel *model);

    void print_error(const std::string &) override;
    void update_for_new_model() override;
    void set_controller(AnalyzerController *controller) override;
};

#endif // QTANALYZERWINDOW_H
