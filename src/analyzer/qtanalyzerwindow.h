#ifndef QTANALYZERWINDOW_H
#define QTANALYZERWINDOW_H

#include "imainwindow.h"

#include <QMainWindow>

class QTreeView;

namespace Ui {
class QtAnalyzerWindow;
}

class QtAnalyzerWindow : public QMainWindow, public IMainWindow
{
    Q_OBJECT

    void perform_search();
    void load_data_file();

public:
    explicit QtAnalyzerWindow(QWidget *parent = 0);
    ~QtAnalyzerWindow();

    void print_error(const std::string &) override {}

    QTreeView *get_tree_view() const;

private:
    Ui::QtAnalyzerWindow *ui;
};

#endif // QTANALYZERWINDOW_H
