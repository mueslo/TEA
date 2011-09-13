#include "finddialog.h"
#include "ui_finddialog.h"
#include "db.h"
#include <QtSql>
#include <QMenu>
#include <QModelIndex>
#include <QTableView>
#include "activeroutelistitem.h"
#include "metadatadialog.h"

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	connectSignalsAndSlots();
	fillComboBoxes();
	metadata = getAllMetadata("rdb");
	displayResult();

}

FindDialog::~FindDialog()
{

}

void FindDialog::displayResult()
{
    ui.lwResult->clear();
	while (metadata.next())
	{
		QListWidgetItem *route = new QListWidgetItem;
		route->setData(Qt::UserRole, metadata.value(0));
		route->setText(metadata.value(5).toString()+" ("+
					QDate::fromString(metadata.value(1).toString(),"ddMMyy").toString("d.M.yy")+
					"): "+QString::number((int)metadata.value(7).toDouble())+"km, +"+
					QString::number(metadata.value(8).toInt()/10)+"m "+
					QString::number(metadata.value(9).toInt()/10)+"m "
					);
		ui.lwResult->addItem(route);
	}

}

void FindDialog::connectSignalsAndSlots()
{
	connect(ui.qyApply, SIGNAL(clicked()), this, SLOT(query()));
	connect(ui.btnLoad, SIGNAL(clicked()), this, SLOT(load()));
	connect(ui.qyClear, SIGNAL(clicked()), this, SLOT(queryAll()));
	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.btnEdtMet, SIGNAL(clicked()), this, SLOT(editMetadata()));
	connect(ui.lwResult, SIGNAL(itemSelectionChanged()), this, SLOT(enableLoadButton()));
	connect(ui.lwResult, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(load()));
	connect(ui.cboxType, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOperators(int)));
	connect(ui.lwResult, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showListContextMenu(const QPoint &)));
	connect(ui.edtValue, SIGNAL(textChanged(QString)), this, SLOT(enableQueryButton(QString)));
}

void FindDialog::showListContextMenu(const QPoint &pos)
{
    QMenu *contextMenu = new QMenu;
    QModelIndex t = ui.lwResult->indexAt(pos);
    contextMenu->addAction(ui.actionLoad_Route);
    contextMenu->addAction(ui.actionEdit_Metadata);
    contextMenu->addAction(ui.actionDelete_Route);

    //ui.lwResult->item(t.row())->setSelected(true); //select despite right click

    contextMenu->exec(ui.lwResult->mapToGlobal(pos));

}

void FindDialog::updateOperators(int index)
{
	ui.cboxOperator->clear();
    switch(index) {
      case 0: //date
    	  ui.cboxOperator->addItem("close to");
    	  ui.cboxOperator->addItem("=");
    	  ui.cboxOperator->addItem(">");
    	  ui.cboxOperator->addItem("<");
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2260")); //!=
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2265")); //>=
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2264")); //<=
    	  break;
      case 1: //id
    	  ui.cboxOperator->addItem("=");
	  break;
      case 2: //location
    	  ui.cboxOperator->addItem("=");
    	  ui.cboxOperator->addItem("contains");
	  break;
      case 3: //tags
    	  ui.cboxOperator->addItem("contain");
    	  break;
      case 4: //name
    	  ui.cboxOperator->addItem("=");
    	  ui.cboxOperator->addItem("contains");
	  break;
      case 5: //distance
    	  ui.cboxOperator->addItem("close to");
    	  ui.cboxOperator->addItem(">");
    	  ui.cboxOperator->addItem("<");
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2265"));
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2264"));
	  break;
      case 6: //altitude gain
    	  ui.cboxOperator->addItem("close to");
    	  ui.cboxOperator->addItem(">");
    	  ui.cboxOperator->addItem("<");
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2265"));
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2264"));
	  break;
      case 7: //loss
    	  ui.cboxOperator->addItem("close to");
    	  ui.cboxOperator->addItem(">");
    	  ui.cboxOperator->addItem("<");
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2265"));
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2264"));
	  break;
      case 8: //alt diff
    	  ui.cboxOperator->addItem("close to");
    	  ui.cboxOperator->addItem(">");
    	  ui.cboxOperator->addItem("<");
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2265"));
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2264"));
	  break;
      case 9: //duration
    	  ui.cboxOperator->addItem("close to");
    	  ui.cboxOperator->addItem(">");
    	  ui.cboxOperator->addItem("<");
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2265"));
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2264"));
	  break;
      case 10: //coordinates
    	  ui.cboxOperator->addItem("near to");
	  break;
      case 11: //mean alt
    	  ui.cboxOperator->addItem("close to");
    	  ui.cboxOperator->addItem(">");
    	  ui.cboxOperator->addItem("<");
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2265"));
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2264"));
	  break;
      case 12: //mean velocity
    	  ui.cboxOperator->addItem("close to");
    	  ui.cboxOperator->addItem(">");
    	  ui.cboxOperator->addItem("<");
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2265"));
    	  ui.cboxOperator->addItem(QString::fromUtf8("\u2264"));
	  break;
      case 13: //near to other route
    	ui.cboxOperator->addItem("near to");
	break;
      default:
	ui.cboxOperator->addItem("Operator");
	break;
    }
}

void FindDialog::query()
{
    int type = ui.cboxType->currentIndex();
    int sign = ui.cboxOperator->currentIndex();
    QString value = ui.edtValue->text();


    /*  The different types
	0 ui.cboxType->addItem("Date (in DDMMYY)");
	1 ui.cboxType->addItem("ID");
	2 ui.cboxType->addItem("Location");
	3 ui.cboxType->addItem("Tags");
	4 ui.cboxType->addItem("Name");
	5 ui.cboxType->addItem("Distance (in km)");
	6 ui.cboxType->addItem("Altitude gain (in m)");
	7 ui.cboxType->addItem("Altitude loss (in m)");
	8 ui.cboxType->addItem("Total altitude difference (in m)");
	9 ui.cboxType->addItem("Duration (in HHMMSS)");
	10 ui.cboxType->addItem("Coordinates (in DDD.DDDDDD°,DD.DDDDDD°)");
	11 ui.cboxType->addItem("Mean altitude (in m)");
	12 ui.cboxType->addItem("Mean velocity (in km/h)");
	13 ui.cboxType->addItem("Route near to Route (in UID)");
      */

    switch(type){
    case 1:
	if (value.isEmpty()) value = "0";
	metadata = customRDBQuery("SELECT * FROM metadata"
				  " WHERE id="+value);
	qDebug("empty");
	if (metadata.isValid()) qDebug(":)");
	break;
    default:
	queryAll();
	break;
    }

    displayResult();
}

void FindDialog::load()
{
	bool exception = false;
	QList<QListWidgetItem*> itemSelection = ui.lwResult->selectedItems();

	for (int i = 0; i<itemSelection.length(); i++)
	{
		if (!rrouteLoadedInADB(itemSelection.at(i)->data(Qt::UserRole).toString()) && !itemSelection.isEmpty())
		{
		    loadRoute(itemSelection.at(i)->data(Qt::UserRole).toString());
		} else exception = true;
	}

	if (exception) QMessageBox::warning(this, "Error", 	"At least one of the routes you selected is either already loaded or you selected no route.\n"
												"The routes that do not fall into this category were loaded.", QMessageBox::Ok);
	FindDialog::accept();
}

void FindDialog::queryAll()
{
	metadata = getAllMetadata("rdb");
	displayResult();
}

void FindDialog::close()
{
	FindDialog::reject();
}

void FindDialog::fillComboBoxes()
{
	ui.cboxType->addItem("Date (in DDMMYY)");
	ui.cboxType->addItem("ID");
	ui.cboxType->addItem("Location");
	ui.cboxType->addItem("Tags");
	ui.cboxType->addItem("Name");
	ui.cboxType->addItem("Distance (in km)");
	ui.cboxType->addItem("Altitude gain (in m)");
	ui.cboxType->addItem("Altitude loss (in m)");
	ui.cboxType->addItem("Total altitude difference (in m)");
	ui.cboxType->addItem("Duration (in HHMMSS)");
	ui.cboxType->addItem("Coordinates (in DDD.DDDDDD°,DD.DDDDDD°)");
	ui.cboxType->addItem("Mean altitude (in m)");
	ui.cboxType->addItem("Mean velocity (in km/h)");
	ui.cboxType->addItem("Route near to Route (in UID)");
}

void FindDialog::enableLoadButton()
{
    ui.btnLoad->setEnabled(ui.lwResult->selectedItems().size() != 0);
}

void FindDialog::enableQueryButton(const QString &text)
{
    ui.qyApply->setEnabled(!text.isEmpty());
}

void FindDialog::editMetadata()
{
    //get selected item
    if( ui.lwResult->selectedItems().size() == 0 )
    {
	qDebug("Nothing selected");
	return;
    }
    //load to adb, get its auid
    QListWidgetItem *Item = ui.lwResult->selectedItems().first();
    if(rrouteLoadedInADB(Item->data(Qt::UserRole).toString()))
    {
	QMessageBox::warning(this, "Error",
			"You have already loaded this Route.\n"
			"Please edit its metadata in the main window.", QMessageBox::Ok);
	return;

    }
    //ActiveRouteListItem *Entry = dynamic_cast<ActiveRouteListItem *>(Item);
    QString auid = loadRoute(Item->data(Qt::UserRole).toString());
    //send auid to metadatadialog
    MetadataDialog d(auid, 1);
    d.exec();
    saveRoute(auid);
    deleteRoute(auid, "adb");
    metadata = getAllMetadata("rdb");
    displayResult();
    //store route in rdb und delete from adb
}
