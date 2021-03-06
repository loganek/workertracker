/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "qtanalyzerwindow.h"
#include "ui_qtanalyzerwindow.h"
#include "ui_aboutdialog.h"
#include "analyzercontroller.h"
#include "wtcommon/datetimeutils.h"

#include <QErrorMessage>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QShortcut>

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

    std::array<QString, 7> days = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    QGridLayout *grid_layout = (QGridLayout*)ui->weekDaysGroupBox->layout();
    for (std::size_t i = 0; i < days.size(); i++)
    {
        weekday_checkbox[i] = new QCheckBox(days[i], this);
        weekday_checkbox[i]->setChecked(true);
        grid_layout->addWidget(weekday_checkbox[i], i / 4, i % 4);
    }

    connect_signals();

    init_secret();
}

void QtAnalyzerWindow::init_secret()
{
    ui->secretExpressionLineEdit->setVisible(false);
    auto secret_shortcut = new QShortcut(QKeySequence("Ctrl+Alt+U"), this);
    secret_shortcut->connect(secret_shortcut, &QShortcut::activated, this, [this] {
        ui->secretExpressionLineEdit->setVisible(true);
    });
    connect(ui->secretExpressionLineEdit, &QLineEdit::returnPressed, this, [this] {
        controller->apply_expression(ui->secretExpressionLineEdit->text().toStdString());
    });
}

void QtAnalyzerWindow::connect_signals()
{
    connect(ui->loadDataFileAction, &QAction::triggered, this, [this] { load_data_file(); });
    connect(ui->searchText, &QLineEdit::textEdited, this, [this] { perform_search(); });
    connect(ui->caseSensitiveCheckBox, &QCheckBox::stateChanged, this, [this] (int) { perform_search(); });

    connect(ui->dateSelectorComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this](int index) {
        set_to_predefined_datetime((PredefinedDateTime)index);
    });

    connect(ui->visualizeCurrentViewAction, &QAction::triggered, this, [this] (bool) {
        controller->open_drilldown_pie_chart();
    });
    connect(ui->aboutAction, &QAction::triggered, this, [this] (bool) {
        auto dialog = new QDialog(this);
        Ui::AboutDialog about_dialog;
        about_dialog.setupUi(dialog); about_dialog.headLabel->setText(WT_PROJECT_NAME_AND_VERSION);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    });

    connect(ui->dayOfWeekAction, &QAction::triggered, this, [this] (bool) {
        controller->open_weekday_of_chart_dialog();
    });
    connect(ui->hourOfDayAction, &QAction::triggered, this, [this] (bool) {
        controller->open_hour_of_chart_dialog();
    });
}

void QtAnalyzerWindow::set_to_predefined_datetime(PredefinedDateTime type)
{
    QDateTime from, to;

    auto current_date = QDate::currentDate();
    switch (type)
    {
    case PredefinedDateTime::TODAY:
        from = (QDateTime)current_date, to = (QDateTime)QDate::currentDate().addDays(1);
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

    ui->toDateTimeEdit->setDateTime(to);
    ui->fromDateTimeEdit->setDateTime(from);
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
    ui->treeView->resizeColumnToContents(0);
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
    set_current_range();
}

void QtAnalyzerWindow::print_error(const std::string &message)
{
    auto msg = new QErrorMessage(this);
    msg->setAttribute(Qt::WA_DeleteOnClose);
    msg->showMessage(QString::fromStdString(message));
}

void QtAnalyzerWindow::on_exitAction_triggered(bool)
{
    QApplication::quit();
}

void QtAnalyzerWindow::on_applyFilterButton_clicked(bool)
{
    set_current_range();
}

void QtAnalyzerWindow::set_current_range()
{
    std::array<bool, 7> weekdays;
    for (std::size_t i = 0; i < weekdays.size(); i++) weekdays[i] = weekday_checkbox[i]->isChecked();

    DataRange data_range(weekdays, ui->fromDateTimeEdit->dateTime().toTime_t(), ui->toDateTimeEdit->dateTime().toTime_t(),
                         ui->fromHourSpinBox->value(), ui->toHourSpinBox->value());
    controller->set_range(data_range);
}

