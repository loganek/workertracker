#include "qtanalyzerwindow.h"
#include "ui_qtanalyzerwindow.h"
#include "qtconfigurationdialog.h"
#include "analyzercontroller.h"

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

    connect(ui->caseSensitiveCheckBox, &QCheckBox::stateChanged, this, [this] (int) {
        perform_search();
    });

    connect(ui->searchText, &QLineEdit::textEdited, this, [this] {
        perform_search();
    });
}

void QtAnalyzerWindow::perform_search()
{
    controller->on_search(ui->searchText->text().toStdString(), ui->caseSensitiveCheckBox->isChecked());
}

QtAnalyzerWindow::~QtAnalyzerWindow()
{
    delete ui;
}

QTreeView* QtAnalyzerWindow::get_tree_view() const
{
    return ui->treeView;
}
