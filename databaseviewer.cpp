#include "databaseviewer.h"
#include "ui_databaseviewer.h"
#include "qsqltablemodel.h"
#include "qmessagebox.h"

DatabaseViewer::DatabaseViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseViewer)
{
    ui->setupUi(this);
    ui->cboxDatabase->addItem("Archived routes (rdb)");
    ui->cboxDatabase->addItem("Loaded routes (adb)");
    ui->cboxDatabase->addItem("Archived maps (mdb)");

    connect(ui->btnQuery, SIGNAL(clicked()), this, SLOT(executeQuery()));


}

void DatabaseViewer::executeQuery()
{
    QString db, table;
    if (ui->cboxDatabase->currentIndex()==0) db = "rdb";
    else {  if (ui->cboxDatabase->currentIndex()==1) db = "adb";
	    else db = "mdb";
	 }
    table = ui->edtQuery->text();

    {
	model = new QSqlTableModel::QSqlTableModel(this,QSqlDatabase::database(db)); //deletes the table after closing the dialog -> prevents the following error: QSqlDatabasePrivate::removeDatabase: connection 'rdb' is still in use, all queries will cease to work.
//	model = new QSqlTableModel::QSqlTableModel(0,QSqlDatabase::database(db));
	//model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	model->setEditStrategy(QSqlTableModel::OnFieldChange);
	model->setTable(table);
	ui->tblvResult->setModel(model);

	model->select();
    }

}

DatabaseViewer::~DatabaseViewer()
{
    delete ui;
}

void DatabaseViewer::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
	ui->retranslateUi(this);
	break;
    default:
	break;
    }
}
