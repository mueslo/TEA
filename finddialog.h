#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QtGui/QDialog>
#include "ui_finddialog.h"
#include <QtSql>

class ListWidgetRouteItem;

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = 0);
    ~FindDialog();

private slots:
    void load();
    void query();
    void queryAll();
    void close();
    void updateOperators(int index);
    void showListContextMenu(const QPoint &);
    void enableLoadButton();
    void editMetadata();

private:
    Ui::FindDialogClass ui;
    void connectSignalsAndSlots();
    void fillComboBoxes();
    void displayResult();
    QSqlQuery metadata;
};

#endif // FINDDIALOG_H
