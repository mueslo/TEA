#include "metadatadialog.h"
#include "db.h"
#include <iostream>
#include "ui_metadatadialog.h"
#include "QtGui/QMessageBox"
#include <QGraphicsPathItem>

using namespace std;

MetadataDialog::MetadataDialog(QString auid, int type, QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	if(type==1) ui.OKButton->setText(tr("&Apply"));
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
	connect(ui.edtName, SIGNAL(textChanged(QString)), this, SLOT(enableLoadButton(QString)));
	connect(ui.OKButton, SIGNAL(clicked()), this, SLOT(save()));
	connect(ui.CancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
}

void MetadataDialog::fillKnownMetadata()
{
	//mpAuid = auid;
	QSqlRecord metadata = getRouteMetadata(mpAuid, "adb");
	qDebug(QString::number(__LINE__));
	renderRoute();
	qDebug("1");
	ui.calDate->setSelectedDate(QDate::fromString(metadata.value(2).toString(), "ddMMyy").addYears(100));
	qDebug("1");
	ui.edtID->setText(metadata.value(3).toString());
	qDebug("1");
	ui.edtLocation->setText(metadata.value(4).toString());
	qDebug("1");
	ui.edtTags->appendPlainText(metadata.value(5).toString());
	qDebug("1");
	ui.edtName->setText(metadata.value(6).toString());
	qDebug("1");
	ui.lblDistance->setText("Distance: " + metadata.value(8).toString() + "km");
	qDebug("1");
	ui.lblAltitudeGain->setText("Altitude gain: " + QString::number(metadata.value(9).toDouble()/10) + "m");
	qDebug("1");
	ui.lblAltitudeLoss->setText("Altitude loss:" + QString::number(metadata.value(10).toDouble()/10) + "m");
	qDebug("1");
	ui.lblTotalAltitudeDifference->setText("Total altitude difference: " + QString::number(metadata.value(11).toDouble()/10) + "m");
	ui.lblAvgAltitude->setText("Average altitude: " + QString::number(metadata.value(17).toDouble()/10) + "m");
	ui.lblAvgVelocity->setText("Average velocity: " + QString::number(metadata.value(18).toDouble()) + "km/h");
	ui.tedtDuration->setTime(QTime::fromString(metadata.value(12).toString(),"HHmmss"));
}

void MetadataDialog::renderRoute()
{
    /*
	QPainterPath path;
	QSqlQuery routeData = getRouteData(mpAuid, "adb");
	QSqlRecord metadata = getRouteMetadata(mpAuid, "adb");
	mvpScene = new QGraphicsScene;
	int nodeSkips = metadata.value(20).toInt()/2500+1;
	ui.graphRoute->setScene(mvpScene);

	ui.graphRoute->setSceneRect(metadata.value(16).toInt(),
				    metadata.value(14).toInt(),
				    metadata.value(15).toInt()-metadata.value(16).toInt(),
				    metadata.value(13).toInt()-metadata.value(14).toInt());
	//ui.graphRoute->fitInView(ui.graphRoute->sceneRect(),Qt::KeepAspectRatioByExpanding);
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
	*/
    mvpScene = new QGraphicsScene;
    ui.graphRoute->setScene(mvpScene);

    QSqlQuery routeData = getRouteData(mpAuid, "adb");
    QSqlRecord metadata = getRouteMetadata(mpAuid, "adb");
    QPainterPath path;
qDebug(QString::number(__LINE__));
    int nodeSkips = metadata.value(20).toInt()/2500+1;
    routeData.first();

    qDebug(QString::number(__LINE__));
    double x,y,tempX,tempY;
    //skip zeroes
    while ((getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString())) == 0) && (nodeNextSkip(routeData,0)));

qDebug(QString::number(__LINE__));
if (routeData.isValid()) {
    x=getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString()));
    y=getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString()));
    qDebug("First coordinate: "+QString::number(x)+"; "+QString::number(y));
    path.moveTo(x,y);
}
qDebug(QString::number(__LINE__));

    while (nodeNextSkip(routeData,nodeSkips))
    {
	    qApp->processEvents();

	    tempX=getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString()));
	    tempY=getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString()));
	    //ui.textInformation->append(QString::number(tempX)+' '+QString::number(tempY));
	    if (nodeNextSkip(routeData,nodeSkips)) {

	    if (((tempY != 0.0) || (tempX != 0.0)) && ((getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString())) != 0) || (getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString())) != 0) ))  {
		x = tempX; y = tempY;
		path.quadTo(x,y,getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString())),getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString())));
		//qDebug(QString::number(x)+" "+QString::number(y)+" "+QString::number(getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString())))+","+QString::number(getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString()))));
	    }
	    }
    }

    /*
    path.moveTo(getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString())),
			    getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString())));

    double x,y;
    while (nodeNextSkip(routeData,nodeSkips))
    {
	    qApp->processEvents();
	    x=getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString()));
	    y=getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString()));
	    if (nodeNextSkip(routeData,nodeSkips)) path.quadTo(x,y,getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString())),getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString())));
    }
    */
    QGraphicsPathItem *pathItem = new QGraphicsPathItem;
    pathItem->setPath(path);
    mvpScene->addItem(pathItem);
    //mvpScene->setSceneRect(-PI,-PI,2*PI,2*PI);
    ui.graphRoute->fitInView(pathItem->boundingRect(), Qt::KeepAspectRatioByExpanding);

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
	//todo: set update variable in pathlistitem to true;
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
