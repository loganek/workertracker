#include "qtanalyzerwindow.h"
#include "ui_qtanalyzerwindow.h"

#include "qtconfigurationdialog.h"

QtAnalyzerWindow::QtAnalyzerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtAnalyzerWindow)
{
    ui->setupUi(this);

    connect(ui->actionConfiguration, &QAction::triggered, this, [this] {
        QtConfigurationDialog* dialog = new QtConfigurationDialog(this);
        dialog->setModal(true);
        dialog->show();
    });
}

QtAnalyzerWindow::~QtAnalyzerWindow()
{
    delete ui;
}

QTreeView* QtAnalyzerWindow::get_tree_view() const
{
    return ui->treeView;
}
