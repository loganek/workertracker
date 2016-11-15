#ifndef QTCONFIGURATIONDIALOG_H
#define QTCONFIGURATIONDIALOG_H

#include <QDialog>

namespace Ui {
class QtConfigurationDialog;
}

class QtConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QtConfigurationDialog(QWidget *parent = 0);
    ~QtConfigurationDialog();

private:
    Ui::QtConfigurationDialog *ui;
};

#endif // QTCONFIGURATIONDIALOG_H
