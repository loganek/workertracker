#include "qtanalyzerwindow.h"
#include "ui_qtanalyzerwindow.h"
#include "analyzercontroller.h"

#include "wtcommon/datetimeutils.h"

#include <QErrorMessage>
#include <QFileDialog>
#include <QLabel>

QtAnalyzerWindow::QtAnalyzerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtAnalyzerWindow)
{
    ui->setupUi(this);

    total_label = new QLabel();
    statusBar()->addWidget(total_label);

    ui->treeView->setSortingEnabled(true);
    ui->loadDataFileAction->setShortcut(Qt::Key_O | Qt::CTRL);
    ui->dateSelectorComboBox->addItems({tr("Today"), tr("Yesterday"), tr("This month"), tr("Last month"), tr("Custom")});

    connect_signals();
}

void QtAnalyzerWindow::connect_signals()
{
    connect(ui->loadDataFileAction, &QAction::triggered, this, [this] { load_data_file(); });
    connect(ui->searchText, &QLineEdit::textEdited, this, [this] { perform_search(); });
    connect(ui->caseSensitiveCheckBox, &QCheckBox::stateChanged, this, [this] (int) { perform_search(); });

    connect(ui->dateSelectorComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this](int index) {
        set_to_predefined_datetime((PredefinedDateTime)index);
    });

    auto on_date_time_changed = [this] (QDateTime) {
        if (set_datetime_lock) return;
        ui->dateSelectorComboBox->setCurrentIndex(PredefinedDateTime::CUSTOM);
        set_period();
    };
    connect(ui->fromDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, on_date_time_changed);
    connect(ui->toDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, on_date_time_changed);
}

void QtAnalyzerWindow::set_to_predefined_datetime(PredefinedDateTime type)
{
    QDateTime from, to;

    auto current_date = QDate::currentDate();
    switch (type)
    {
    case PredefinedDateTime::TODAY:
        from = (QDateTime)current_date, to = QDateTime::currentDateTime();
        break;
    case PredefinedDateTime::YESTERDAY:
        from = (QDateTime)current_date.addDays(-1), to = (QDateTime)current_date;
        break;
    case PredefinedDateTime::THIS_MONTH:
        from = (QDateTime)QDate(current_date.year(), current_date.month(), 1), to = QDateTime::currentDateTime();
        break;
    case PredefinedDateTime::LAST_MONTH:
    {
        auto this_month = QDate(current_date.year(), current_date.month(), 1);
        from = (QDateTime)this_month.addMonths(-1), to = (QDateTime)this_month;
        break;
    }
    default:
        return;
    }

    set_datetime_lock = true;
    ui->toDateTimeEdit->setDateTime(to);
    ui->fromDateTimeEdit->setDateTime(from);
    set_datetime_lock = false;

    set_period();
}

void QtAnalyzerWindow::set_period()
{
	WT::DateRange range;
	range.from = ui->fromDateTimeEdit->dateTime().toTime_t();
	range.to = ui->toDateTimeEdit->dateTime().toTime_t();
    controller->set_period(range);
}

void QtAnalyzerWindow::perform_search()
{
    controller->apply_filter(ui->searchText->text().toStdString(), ui->caseSensitiveCheckBox->isChecked());
}

void QtAnalyzerWindow::update_total_time(const std::chrono::seconds& update_total_time)
{
    total_label->setText(QString(tr("Total time: %1")).arg(QString::fromStdString(
                                                           WT::time_to_display(update_total_time))));
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
}

QtAnalyzerWindow::~QtAnalyzerWindow()
{
    delete ui;
}

void QtAnalyzerWindow::set_model(QAbstractItemModel *model)
{
    ui->treeView->setModel(model);
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
