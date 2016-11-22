#include "qtanalyzerwindow.h"
#include "ui_qtanalyzerwindow.h"
#include "qtconfigurationdialog.h"
#include "analyzercontroller.h"

#include <QErrorMessage>
#include <QFileDialog>
#include <QLabel>

QStringList PredefinedDateTime::values = {"Today", "Yesterday", "This month", "Last month", "Custom"};

QtAnalyzerWindow::QtAnalyzerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtAnalyzerWindow)
{
    ui->setupUi(this);

    ui->treeView->setSortingEnabled(true);

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

    connect(ui->fromDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, [this] (QDateTime) {
        if (set_datetime_transaction) return;
        ui->dateSelectorComboBox->setCurrentIndex(PredefinedDateTime::CUSTOM);
        set_current_datetime();
    });

    connect(ui->toDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, [this] (QDateTime) {
        if (set_datetime_transaction) return;
        ui->dateSelectorComboBox->setCurrentIndex(PredefinedDateTime::CUSTOM);
        set_current_datetime();
    });

    connect(ui->searchText, &QLineEdit::textEdited, this, [this] {
        perform_search();
    });

    ui->dateSelectorComboBox->addItems(PredefinedDateTime::values);
    connect(ui->dateSelectorComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this](int index) {
        set_to_predefined_datetime((PredefinedDateTime::Type)index);
    });
}

void QtAnalyzerWindow::set_to_predefined_datetime(PredefinedDateTime::Type type)
{
    auto set_datetime_controls = [this] (QDateTime from, QDateTime to) {
        ui->toDateTimeEdit->setDateTime(to);
        ui->fromDateTimeEdit->setDateTime(from);
    };

    set_datetime_transaction = true;

    auto current_date = QDate::currentDate();
    switch (type)
    {
    case PredefinedDateTime::TODAY:
        set_datetime_controls((QDateTime)current_date, QDateTime::currentDateTime());
        break;
    case PredefinedDateTime::YESTERDAY:
        set_datetime_controls((QDateTime)current_date.addDays(-1), (QDateTime)current_date);
        break;
    case PredefinedDateTime::THIS_MONTH:
        set_datetime_controls((QDateTime)QDate(current_date.year(), current_date.month(), 1), QDateTime::currentDateTime());
        break;
    case PredefinedDateTime::LAST_MONTH:
    {
        auto this_month = QDate(current_date.year(), current_date.month(), 1);
        set_datetime_controls((QDateTime)this_month.addMonths(-1), (QDateTime)this_month);
        break;
    }
    default:
        set_datetime_transaction = false;
        return;
    }

    set_datetime_transaction = false;
    set_current_datetime();
}

void QtAnalyzerWindow::set_current_datetime()
{
    controller->set_period({ui->fromDateTimeEdit->dateTime().toTime_t(), ui->toDateTimeEdit->dateTime().toTime_t()});
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

void QtAnalyzerWindow::set_controller(AnalyzerController *controller)
{
    IMainWindow::set_controller(controller);

    auto date_time = PredefinedDateTime::TODAY;
    ui->dateSelectorComboBox->setCurrentIndex((int)date_time);
    set_to_predefined_datetime(date_time);
}

void QtAnalyzerWindow::print_error(const std::string &message)
{
    QErrorMessage msg;
    msg.showMessage(QString::fromStdString(message));
}
