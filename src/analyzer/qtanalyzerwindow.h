#ifndef QTANALYZERWINDOW_H
#define QTANALYZERWINDOW_H

#include "imainwindow.h"

#include <QMainWindow>

class QTreeView;
class QLabel;

namespace Ui {
class QtAnalyzerWindow;
}

struct PredefinedDateTime
{
    static QStringList values;

    enum Type
    {
        TODAY = 0,
        YESTERDAY,
        THIS_MONTH,
        LAST_MONTH,
        CUSTOM
    };
};

class QtAnalyzerWindow : public QMainWindow, public IMainWindow
{
    Q_OBJECT

    QLabel *total_label;

    void perform_search();
    void load_data_file();
    void update_total_time();

    std::atomic_bool set_datetime_transaction;
    void set_current_datetime();
    void set_to_predefined_datetime(PredefinedDateTime::Type type);

public:
    explicit QtAnalyzerWindow(QWidget *parent = 0);
    ~QtAnalyzerWindow();

    void print_error(const std::string &) override;

    QTreeView *get_tree_view() const;

    void update_for_new_model() override;

    void set_controller(AnalyzerController *controller) override;

private:
    Ui::QtAnalyzerWindow *ui;
};

#endif // QTANALYZERWINDOW_H
