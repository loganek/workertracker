#ifndef QTANALYZERWINDOW_H
#define QTANALYZERWINDOW_H

#include "imainwindow.h"

#include <QMainWindow>

class QTreeView;
class QLabel;

namespace Ui {
class QtAnalyzerWindow;
}

class QtAnalyzerWindow : public QMainWindow, public IMainWindow
{
    Q_OBJECT

    QLabel *total_label;

    void perform_search();
    void load_data_file();
    void update_total_time();

public:
    explicit QtAnalyzerWindow(QWidget *parent = 0);
    ~QtAnalyzerWindow();

    void print_error(const std::string &) override {}

    QTreeView *get_tree_view() const;

    void update_for_new_model() override;

private:
    Ui::QtAnalyzerWindow *ui;
};

#endif // QTANALYZERWINDOW_H
