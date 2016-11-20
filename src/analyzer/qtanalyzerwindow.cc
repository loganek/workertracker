#include "qtanalyzerwindow.h"
#include "ui_qtanalyzerwindow.h"
#include "qtconfigurationdialog.h"
#include "analyzercontroller.h"

#include <QFileDialog>
#include <QLabel>

QtAnalyzerWindow::QtAnalyzerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtAnalyzerWindow)
{
    ui->setupUi(this);

    total_label = new QLabel();
    statusBar()->addWidget(total_label);

    ui->loadDataFileAction->setShortcut(Qt::Key_O | Qt::CTRL);
    connect(ui->loadDataFileAction, &QAction::triggered, this, [this] {
        load_data_file();
    });

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
    update_total_time();
}

void QtAnalyzerWindow::update_total_time()
{
    total_label->setText(QString("Total time: %1").arg(QString::fromStdString(
                                                           AnalyzerController::time_to_display(controller->get_total_time()))));
}

void QtAnalyzerWindow::load_data_file()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open Data File"), "", tr("Data Files (*.dat);; All Files (*)"));

    if (!file_name.isEmpty()&& !file_name.isNull())
    {
        controller->load_from_file(file_name.toStdString());
    }
}

void QtAnalyzerWindow::update_for_new_model()
{
    ui->treeView->expandAll();
    ui->treeView->resizeColumnToContents(1);
    ui->treeView->hideColumn(2);

    // TODO perform_search should be requested by controller
    perform_search();
}

QtAnalyzerWindow::~QtAnalyzerWindow()
{
    delete ui;
}

QTreeView* QtAnalyzerWindow::get_tree_view() const
{
    return ui->treeView;
}
