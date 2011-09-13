#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    qDebug("SettingsDialog created");
}

SettingsDialog::~SettingsDialog()
{
    qDebug("SettingsDialog destroyed");
    delete ui;
}
