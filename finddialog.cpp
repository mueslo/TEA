#include "finddialog.h"
#include "ui_finddialog.h"
#include "db.h"
#include <QtSql>
#include <QTableView>

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
	connect(ui.bboxQuery, SIGNAL(accepted()), this, SLOT(query()));
	connect(ui.bboxChoice, SIGNAL(accepted()), this, SLOT(load()));
	connect(ui.bboxQuery, SIGNAL(rejected()), this, SLOT(queryAll()));
	connect(ui.bboxChoice, SIGNAL(rejected()), this, SLOT(close()));
	connect(ui.cboxType, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOperators(int)));
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

}

void FindDialog::load()
{
	bool exception = false;
	QList<QListWidgetItem*> itemSelection = ui.lwResult->selectedItems();
	cout << "UID" << itemSelection.at(0)->data(Qt::UserRole).toString().toStdString() << endl;

	for (int i = 0; i<itemSelection.length(); i++)
	{
		if (!rrouteLoadedInADB(itemSelection.at(i)->data(Qt::UserRole).toString()) && !itemSelection.isEmpty())
		{
			cout << "loading a route" << endl; //todo rm
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
