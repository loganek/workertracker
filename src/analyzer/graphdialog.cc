/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "graphdialog.h"
#include "smartpieseries.h"
#include "ui_graphdialog.h"

#include "drilldownslice.h"
#include "qtfilterproxymodel.h"

QT_CHARTS_USE_NAMESPACE

GraphDialog::GraphDialog(const WT::ProcNameGroupPolicy::container_t& container, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphDialog)
{
    ui->setupUi(this);

    chart = new DrilldownChart(update_policy(), container);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QChart::AllAnimations);

    auto chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->verticalLayout->addWidget(chartView);

    connect(ui->overviewModeRadioButton, &QRadioButton::clicked, this, [this] (bool) { chart->set_model_type(is_full_mode());});
    connect(ui->fullModeRadioButton, &QRadioButton::clicked, this, [this] (bool) { chart->set_model_type(is_full_mode());});

    connect(ui->maxCntPolicyRadioButton, &QRadioButton::clicked, this, &GraphDialog::policy_updated);
    connect(ui->minPercentagePolicyRadioButton, &QRadioButton::clicked, this, &GraphDialog::policy_updated);
    connect(ui->allPolicyRadioButton, &QRadioButton::clicked, this, &GraphDialog::policy_updated);

    connect(ui->minPercentageDoubleSpinBox, &QDoubleSpinBox::editingFinished, this, [this] {policy_updated();});
    connect(ui->maxCountSpinBox, &QSpinBox::editingFinished, this, [this] {policy_updated();});
}

GraphDialog::~GraphDialog()
{
    delete ui;
}

bool GraphDialog::is_full_mode() const
{
    return ui->fullModeRadioButton->isChecked();
}

void GraphDialog::policy_updated(bool)
{
    chart->set_policy(update_policy());
}

std::shared_ptr<PieSeriesPolicy> GraphDialog::update_policy()
{
    std::shared_ptr<PieSeriesPolicy> policy;
    bool percentage_enabled = false, count_enabled = false;

    if (ui->maxCntPolicyRadioButton->isChecked())
    {
        policy = std::make_shared<MaxCountPieSeriesPolicy>(ui->maxCountSpinBox->value());
        count_enabled = true;
    }
    else if (ui->minPercentagePolicyRadioButton->isChecked())
    {
        policy = std::make_shared<MinPercentagePieSeriesPolicy>(ui->minPercentageDoubleSpinBox->value() / 100, 0);
        percentage_enabled = true;
    }
    else
    {
        policy = std::make_shared<AllPieSeriesPolicy>();
    }

    ui->maxCountSpinBox->setEnabled(count_enabled);
    ui->minPercentageDoubleSpinBox->setEnabled(percentage_enabled);

    return policy;
}
