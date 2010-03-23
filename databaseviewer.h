#ifndef DATABASEVIEWER_H
#define DATABASEVIEWER_H

#include <QDialog>
#include "qsqltablemodel.h"

namespace Ui {
    class DatabaseViewer;
}

class DatabaseViewer : public QDialog {
    Q_OBJECT
public:
    DatabaseViewer(QWidget *parent = 0);
    ~DatabaseViewer();

protected:
    void changeEvent(QEvent *e);

private slots:
    void executeQuery();

private:
    Ui::DatabaseViewer *ui;
    QSqlTableModel *model;
};

#endif // DATABASEVIEWER_H
