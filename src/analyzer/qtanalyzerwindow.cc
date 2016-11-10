#include "qtanalyzerwindow.h"
#include "ui_qtanalyzerwindow.h"

QtAnalyzerWindow::QtAnalyzerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtAnalyzerWindow)
{
    ui->setupUi(this);
}

QtAnalyzerWindow::~QtAnalyzerWindow()
{
    delete ui;
}

QTreeView* QtAnalyzerWindow::get_tree_view() const
{
    return ui->treeView;
}
