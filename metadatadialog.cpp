#include "metadatadialog.h"
#include "db.h"
#include <iostream>
#include "ui_metadatadialog.h"
#include "QtGui/QMessageBox"

using namespace std;

MetadataDialog::MetadataDialog(QString auid, QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	connectSignalsAndSlots();
	mpAuid = auid;
	fillKnownMetadata();
}

MetadataDialog::~MetadataDialog()
{
	delete mvpScene;
}

void MetadataDialog::connectSignalsAndSlots()
{
	connect(ui.btnZoomIn, SIGNAL(clicked()), this, SLOT(zoomIn()));
	connect(ui.btnZoomOut, SIGNAL(clicked()), this, SLOT(zoomOut()));
	connect(ui.btnbResponse, SIGNAL(accepted()), this, SLOT(save()));
	connect(ui.btnbResponse, SIGNAL(rejected()), this, SLOT(cancel()));
}

void MetadataDialog::fillKnownMetadata()
{
	//mpAuid = auid;
	QSqlRecord metadata = getRouteMetadata(mpAuid, "adb");
	//QSqlRecord metadata = db.getRouteMetadataFromADB(mpAuid);
	renderRoute();
	ui.calDate->setSelectedDate(QDate::fromString(metadata.value(2).toString(), "ddMMyy").addYears(100));
	ui.edtID->setText(metadata.value(3).toString());
	ui.edtLocation->setText(metadata.value(4).toString());
	ui.edtTags->appendPlainText(metadata.value(5).toString());
	ui.edtName->setText(metadata.value(6).toString());
	ui.lblDistance->setText("Distance: " + metadata.value(8).toString() + "km");
	ui.lblAltitudeGain->setText("Altitude gain: " + QString::number(metadata.value(9).toDouble()/10) + "m");
	ui.lblAltitudeLoss->setText("Altitude loss:" + QString::number(metadata.value(10).toDouble()/10) + "m");
	ui.lblTotalAltitudeDifference->setText("Total altitude difference: " + QString::number(metadata.value(11).toDouble()/10) + "m");
	ui.lblAvgAltitude->setText("Average altitude: " + QString::number(metadata.value(17).toDouble()/10) + "m");
	ui.lblAvgVelocity->setText("Average velocity: " + QString::number(metadata.value(18).toDouble()) + "km/h");
	ui.tedtDuration->setTime(QTime::fromString(metadata.value(12).toString(),"HHmmss"));
}

void MetadataDialog::renderRoute()
{
	QPainterPath path;
	QSqlQuery routeData = getRouteData(mpAuid, "adb");
	QSqlRecord metadata = getRouteMetadata(mpAuid, "adb");
	mvpScene = new QGraphicsScene;
	int nodeSkips = metadata.value(20).toInt()/2500+1;
	ui.graphRoute->setScene(mvpScene);
	ui.graphRoute->setSceneRect(metadata.value(16).toInt(),metadata.value(14).toInt(),metadata.value(15).toInt()-metadata.value(16).toInt(),metadata.value(13).toInt()-metadata.value(14).toInt());
	ui.graphRoute->fitInView(ui.graphRoute->sceneRect(),Qt::KeepAspectRatioByExpanding);
	ui.graphRoute->show();
	routeData.first();
	path.moveTo(getXFromLon(routeData.value(5).toString()), getYFromLat(routeData.value(4).toString()));
	int x,y;
	while (nodeNextSkip(routeData,nodeSkips))
	{
		qApp->processEvents();
		x=getXFromLon(routeData.value(5).toString());
		y=getYFromLat(routeData.value(4).toString());
		if (nodeNextSkip(routeData,nodeSkips)) path.quadTo(x,y,getXFromLon(routeData.value(5).toString()),getYFromLat(routeData.value(4).toString()));
	}
	mvpScene->addPath(path, QPen(Qt::black), QBrush(Qt::transparent));
}

bool MetadataDialog::nodeNextSkip(QSqlQuery routeData, int timesToSkip)
{
	int i;
	for(i=1; i<timesToSkip; i++) routeData.next();
	return routeData.next();
}


void MetadataDialog::zoomIn()
{
	rerenderRoute(1.25,1.25);
}

void MetadataDialog::zoomOut()
{
	rerenderRoute(0.8,0.8);
}

void MetadataDialog::rerenderRoute(double zoomX, double zoomY)
{
	ui.graphRoute->scale(zoomX, zoomY);
}

void MetadataDialog::save()
{
	cout << "chosen to save" << endl;
	//Write metadata to adb
	setRouteName(mpAuid, ui.edtName->text());
	setRouteID(mpAuid, ui.edtID->text());
	setRouteLocation(mpAuid, ui.edtLocation->text());
	setRouteTags(mpAuid, ui.edtTags->document()->toPlainText());
	//Save DB entry to rdb
	saveRoute(mpAuid);
	//Ask to remove DB entry from adb
	//close dialog
	MetadataDialog::accept();
}

void MetadataDialog::cancel()
{
	//ask if sure
	//Remove DB entry from adb
	//close dialog
	MetadataDialog::reject();
}
