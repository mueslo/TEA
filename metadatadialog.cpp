#include "metadatadialog.h"
#include "db.h"
#include <iostream>
#include "ui_metadatadialog.h"
#include "QtGui/QMessageBox"
#include <QGraphicsPathItem>

using namespace std;

MetadataDialog::MetadataDialog(QString auid, int type, QWidget *parent)
    : QDialog(parent), mpAuid(auid)
{
	ui.setupUi(this);
	if(type==1) ui.OKButton->setText(tr("&Apply"));
	connectSignalsAndSlots();
        ui.graphRoute->setMapSource(Mapnik);
        ui.graphRoute->setZoomable(false);          //prevents zooming
        ui.graphRoute->setFocusPolicy(Qt::NoFocus); //prevents scrolling by keyboard
        ui.graphRoute->setMouseTracking(false);     //prevents scrolling by mouse
        fillKnownMetadata();
}

MetadataDialog::~MetadataDialog()
{
   ui.graphRoute->remove(route);
   delete route;
}

void MetadataDialog::connectSignalsAndSlots()
{
	connect(ui.edtName, SIGNAL(textChanged(QString)), this, SLOT(enableLoadButton(QString)));
	connect(ui.OKButton, SIGNAL(clicked()), this, SLOT(save()));
        connect(ui.CancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
}

void MetadataDialog::resizeEvent(QResizeEvent* event)
{
    qDebug("resize");
    ui.graphRoute->centerMapOnRoute(route);
}

void MetadataDialog::fillKnownMetadata()
{
	QSqlRecord metadata = getRouteMetadata(mpAuid, "adb");

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
        renderRoute();
}

void MetadataDialog::renderRoute()
{
    QSqlQuery routedata = getRouteData(mpAuid, "adb");
    QSqlRecord metadata = getRouteMetadata(mpAuid, "adb");
    routedata.first();

    qDebug("Generating path.");
    int nodeSkips = metadata.value(20).toInt()/2500+1;
    QPainterPath path;

    routedata.first();

    double x,y,tempX,tempY;
    //skip zeroes

    while ((getMercatorYFromLat(getLatFromRawLat(routedata.value(4).toString())) == 0) && (nodeNextSkip(routedata,0)));

    if (routedata.isValid()) {
        x=getMercatorXFromLon(getLonFromRawLon(routedata.value(5).toString()));
        y=getMercatorYFromLat(getLatFromRawLat(routedata.value(4).toString()));
        qDebug("First coordinate: "+QString::number(x)+"; "+QString::number(y));
        path.moveTo(x,y);
    }

    /* Draw Path */
    while (nodeNextSkip(routedata,nodeSkips))
    {
        qApp->processEvents();

        tempX=getMercatorXFromLon(getLonFromRawLon(routedata.value(5).toString()));
        tempY=getMercatorYFromLat(getLatFromRawLat(routedata.value(4).toString()));
        if( ( tempX != 0 ) || ( tempY != 0) ) {
            path.lineTo(tempX, tempY);
        }
    }
    QGraphicsPathItem *pathItem = new QGraphicsPathItem;
    pathItem->setPath(path);
    route = new ActiveRouteListItem("",0);
    route->setPath(pathItem);

    ui.graphRoute->add(route);
    //warning: ui.graphRoute is online minimum size here, if min. size is set too small, it only
    // shows the route as quite small
    ui.graphRoute->centerMapOnRoute(route);
    qDebug("mdd width: "+QString::number(ui.graphRoute->width())+" ("+QString::number(__LINE__)+")");

}



bool MetadataDialog::nodeNextSkip(QSqlQuery routeData, int timesToSkip)
{
	int i;
	for(i=1; i<timesToSkip; i++) routeData.next();
	return routeData.next();
}

void MetadataDialog::save()
{
	cout << "chosen to save" << endl;
	//Write metadata to adb
	setRouteName(mpAuid, ui.edtName->text());
	setRouteID(mpAuid, ui.edtID->text());
	setRouteLocation(mpAuid, ui.edtLocation->text());
	setRouteTags(mpAuid, ui.edtTags->document()->toPlainText());
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

void MetadataDialog::enableLoadButton(const QString &text)
{
    ui.OKButton->setEnabled(!text.isEmpty());
}
