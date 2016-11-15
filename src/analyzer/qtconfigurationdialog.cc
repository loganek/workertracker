#include "qtconfigurationdialog.h"
#include "ui_qtconfigurationdialog.h"

QtConfigurationDialog::QtConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtConfigurationDialog)
{
    ui->setupUi(this);
}

QtConfigurationDialog::~QtConfigurationDialog()
{
    delete ui;
}
