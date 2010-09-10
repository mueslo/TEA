/*
 * maintea.cpp
 *
 *  Created on: 27.08.2009
 *      Author: jf
 */

#include <iostream>
#include <stdio.h>
#include <QtGui>
#include <QApplication>
#include <QtNetwork>
#include <QFile>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "ui_tea.h"
#include "tea.h"
#include <stdlib.h>
#include "db.h"
#include "metadatadialog.h"
#include "finddialog.h"
#include "databaseviewer.h"
#include "qwt_plot.h"
#include "qwt_data.h"
#include "qwt_plot_curve.h"
#include "pathlist.h"


#define PI 3.1415926535897932384626433832795

using namespace std;

TEA::TEA(QWidget *parent) :
	QMainWindow(parent)
{
	init = true;

	if (!initialiseDBs())
	{
	    QMessageBox::critical(this, tr("A critical error occured!"),
				  "A critical error occured while intialising active routes",
				  QMessageBox::Abort | QMessageBox::Ignore);
	}

	networkManager = new QNetworkAccessManager(this);

	connect(networkManager, SIGNAL(finished(QNetworkReply*)),
		 this, SLOT(downloaded(QNetworkReply*)));
	ui.setupUi(this);
	ui.textInformation->append("<b>TEA console</b>");
	scene = new QGraphicsScene;

	//tile size: 256px x 256px, in coordinates: 2*PI x 2*PI
	ui.graphicsView->scale(128/PI,128/PI);
	mapSource = "http://andy.sandbox.cloudmade.com/tiles/cycle/";
	getTile(0,0,0);
	//initially zoom always = 0
	zoomOld = 0;
	ui.graphicsView->setScene(scene);
	connectSignalsAndSlots();
	createToolBar();
	createStatusBar();
	fillTrainerViewCBoxes();

	QwtPlot *trainer = new QwtPlot;
	trainer->setAxisAutoScale(true);
	trainer->setAxisTitle(0,"test");

	/*
	QProgressBar *progress = new QProgressBar(ui.statusbar);
	progress->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	ui.statusbar->addPermanentWidget(progress);
	progress->setAlignment(Qt::AlignRight);
	*/

	/*
	if (!db.createAndLoadActiveRoutesDB())
	{
	     QMessageBox::critical(this, tr("A critical error occured!"),
					     "A critical error occured while intialising active routes (new)",
					     QMessageBox::Abort | QMessageBox::Ignore);
	}*/
}

TEAView::TEAView(QWidget *parent) : QGraphicsView(parent)
{

}

void TEAView::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta()/8;
	int numSteps = numDegrees/15;
	if (event->orientation() == Qt::Vertical)
	{
		emit wheelZoom(numSteps);
	}
}


void TEA::createStatusBar()
{
	prgBar = new QProgressBar(ui.statusbar);
	prgBar->setAlignment(Qt::AlignRight);
	prgBar->setGeometry(0,0,100,10);
	ui.statusbar->addPermanentWidget(prgBar);
	ui.statusbar->showMessage(tr("Ready"));
}

void TEA::fillTrainerViewCBoxes()
{
	ui.cboxX->clear();
	ui.cboxY->clear();

	if (ui.rbNode->isChecked())
	{
		ui.cboxY->addItem("Distance");
		ui.cboxY->addItem("Altitude");
		ui.cboxY->addItem("Velocity");
		ui.cboxY->addItem("Slope");
		ui.cboxY->addItem("Pedal frequency");

		ui.cboxX->addItem("Time");
		ui.cboxX->addItem("Distance");
	} else {
	ui.cboxY->addItem("Mean velocity");
	ui.cboxY->addItem("Mean altitude");
	ui.cboxY->addItem("Duration");
	ui.cboxY->addItem("Altitude gain");
	ui.cboxY->addItem("Altitude loss");


	ui.cboxX->addItem("Date");
	ui.cboxX->addItem("Duration");
	ui.cboxX->addItem("Mean velocity");
	ui.cboxX->addItem("Mean altitude");
	ui.cboxX->addItem("Altitude gain");
	ui.cboxX->addItem("Altitude loss");

	}
}

void TEA::resizeEvent(QResizeEvent* event)
{
	if (init == false)
	{
		scene->setSceneRect(-PI,-PI,2*PI,2*PI);
	} else {
		cout << "graphicsView initialised" << endl; //todo rm
		ui.textInformation->append("graphicsView initialised"); //todo rm
		init = false;
	}

}

void TEA::viewChange()
{
	getTilesInRange();
}

void TEA::getTilesInRange()
{
	QRectF viewRect = ui.graphicsView->mapToScene(0,0,ui.graphicsView->width(),ui.graphicsView->height()).boundingRect();
	QList<QPoint> XYTileList = getXYTileInRange(ui.sldZoom->value(),
									getLongFromMercatorX(viewRect.x()+viewRect.width()),
									getLongFromMercatorX(viewRect.x()),
									getLatFromMercatorY(viewRect.y()+viewRect.height()),
									getLatFromMercatorY(viewRect.y()));
	for(int i=0;i<XYTileList.size();i++){ getTile(XYTileList.at(i).x(),XYTileList.at(i).y(),ui.sldZoom->value());
	}


}

void TEA::placeTile(QByteArray tile, int tileX, int tileY, int zoomLevel)
{
	QPixmap tilePixmap;
	tilePixmap.loadFromData(tile);
	QGraphicsPixmapItem *tilePixmapItem = new QGraphicsPixmapItem;
	tilePixmapItem->setPixmap(tilePixmap);
	tilePixmapItem->setPos(tileToCoord(tileX,tileY,zoomLevel));
	tilePixmapItem->scale(PI / pow(2.0, 7 + zoomLevel),PI / pow(2.0, 7 + zoomLevel));
	tilePixmapItem->setZValue(zoomLevel);
	scene->addItem(tilePixmapItem);
}

void TEA::getTile(int tileX, int tileY, int zoomLevel)
{
	//todo: check if present in DB
	QByteArray tile = getTileFromDB(zoomLevel,tileX,tileY);
	//if ((QString::fromAscii(tile)!="0") ) placeTile(tile, tileX, tileY, zoomLevel);
	if (QString::fromAscii(tile)!="0"){
	    placeTile(tile, tileX, tileY, zoomLevel);
	    qDebug("Tile from DB");
	}
	else if(ui.sldZoom->value() == zoomLevel)
	{
	    networkManager->get(QNetworkRequest(QUrl(qPrintable(
		mapSource+QString::number(zoomLevel)+"/"+QString::number(tileX)+"/"+QString::number(tileY)+".png"))));
	}
	 //manager->get(QNetworkRequest(QUrl("http://tile.openstreetmap.org/cgi-bin/export?bbox=7.28,48.35,7.29,48.36&scale=10000&format=png")));

}

void TEA::downloaded(QNetworkReply* reply)
{
	QByteArray data = reply->readAll();
	QString path = reply->url().path();
	path.chop(4);
	if (ui.rbCycleMap->isChecked()) path.remove(0,12);

	int zoomLevel = path.section('/',1,1).toInt();
	int tileX = path.section('/',2,2).toInt();
	int tileY = path.section('/',3,3).toInt();

	//todo: functioning addtiletodb
	addTileToDB(zoomLevel,tileX,tileY,data);

	//IF-Bedingung ist mehr oder weniger QND. ;)
	//BTW. ich zweifle bischen dran dass, wenn du die downloadaufforderung einmal an den QNetworkManager geschickt hast, du die abarbeitungsreihenfolge noch ändern kannst.
	if( ui.sldZoom->value() == zoomLevel ) placeTile(data, tileX, tileY, zoomLevel);

	//newTile->setPos(-PI, -PI);
	//newTile->scale(PI/128, PI/128);
	ui.textInformation->append("Tile image file size:" + QString::number(data.length()) + " bits (x:"+QString::number(tileX)+" y:"+QString::number(tileY)+" z:"+QString::number(zoomLevel)+")");
	//scene->addItem(newTile);

	//set position:

	/*//WITH FILE HANDLING
	QString fileName = QString::fromUtf8("temp");
    if (QFile::exists(fileName)) {
	if (QMessageBox::question(this, tr("HTTP"),
				  tr("There already exists a file called %1 in "
				     "the current directory. Overwrite?").arg(fileName),
				  QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
	    == QMessageBox::No)
	    return;
	QFile::remove(fileName);
    }

	file = new QFile(fileName);

    if (!file->open(QIODevice::WriteOnly)) {
	QMessageBox::information(this, tr("HTTP"),
				 tr("Unable to save the file %1: %2.")
				 .arg(fileName).arg(file->errorString()));
	delete file;
	file = 0;
	return;
    }

	file->write(data);
	file->close();
	*/
	reply->deleteLater();
}

void TEA::createToolBar()
{
	ui.tbMain->addAction(ui.loadFromFileAction);
	ui.tbMain->addAction(ui.loadFromDatabaseAction);
	ui.tbMain->addSeparator();
	ui.tbMain->addAction(ui.saveAction);
	ui.tbMain->addAction(ui.saveAllAction);
	ui.tbMain->addSeparator();
	ui.tbMain->addAction(ui.actionEdit_metadata);
	ui.tbMain->addAction(ui.actionCenter_Map);
}

void TEA::rotateClockwise()
{
	ui.graphicsView->rotate(-20);
}

void TEA::rotateCClockwise()
{
	ui.graphicsView->rotate(20);
}

void TEA::connectSignalsAndSlots()
{
	connect(ui.loadFromFileAction, SIGNAL(triggered()), this, SLOT(loadFromFile()));
	connect(ui.loadFromDatabaseAction, SIGNAL(triggered()), this, SLOT(loadFromDatabase()));
	connect(ui.exitAction, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.aboutAction, SIGNAL(triggered()), this, SLOT(About()));
	connect(ui.btnZoomIn, SIGNAL(clicked()), this, SLOT(zoomIn()));
	connect(ui.btnZoomOut, SIGNAL(clicked()), this, SLOT(zoomOut()));
	connect(ui.sldZoom, SIGNAL(valueChanged(int)), this, SLOT(sldChanged(int)));
	connect(ui.refreshMapAction, SIGNAL(triggered()), this, SLOT(viewChange()));
	connect(ui.cbtnRotateClockwise, SIGNAL(clicked()), this, SLOT(rotateClockwise()));
	connect(ui.cbtnRotateCClockwise, SIGNAL(clicked()), this, SLOT(rotateCClockwise()));
	connect(ui.dwBottom, SIGNAL(visibilityChanged(bool)), this, SLOT(consoleChanged()));
	connect(ui.consoleAction, SIGNAL(triggered()), this, SLOT(consoleButtonTriggered()));
	connect(ui.rbMapnik, SIGNAL(toggled(bool)), this, SLOT(mapSourceChanged()));
	connect(ui.cboxX, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerSelectionChange()));
	connect(ui.cboxY, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerSelectionChange()));
	connect(ui.rbNode, SIGNAL(toggled(bool)), this, SLOT(trainerModeChanged()));
	connect(ui.graphicsView, SIGNAL(wheelZoom(int)), this, SLOT(zoom(int)));
	connect(ui.btnGeneralSettings, SIGNAL(clicked()), this, SLOT(setGeneralSettings()));
	connect(ui.databaseViewAction, SIGNAL(triggered()), this, SLOT(actionViewDatabase()));
	connect(ui.listWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(updatePath(QListWidgetItem*)));
	//connect(ui.saveAction, SIGNAL(triggered()), this, SLOT(UpdateADB()) );
	connect(ui.saveAction, SIGNAL(triggered()), this, SLOT(saveToDatabase()));
	connect(ui.actionEdit_metadata, SIGNAL(triggered()), this, SLOT(editMetadata()));
	connect(ui.actionCenter_Map, SIGNAL(triggered()), this, SLOT(centerMapOnSelectedRoute()));
	connect(ui.listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showListContextMenu(const QPoint &)));
	//connect(ui.graphicsView, SIGNAL(resizeEvent()), this, SLOT(graphicsViewResized()));
	//connect(ui.graphicsView, SIGNAL(mousePressed()), this, SLOT(grphPressed()));
	//ui.tbMain->addAction(QIcon("icons/32x32_0560/map.png"), "Something with maps", this, "mapAction");
}


void TEA::showListContextMenu(const QPoint &pos)
{
    QMenu *contextMenu = new QMenu;
    QModelIndex t = ui.listWidget->indexAt(pos);
    contextMenu->addAction(ui.saveAction);
    contextMenu->addAction(ui.actionCenter_Map);
    contextMenu->addAction(ui.actionEdit_metadata);
    contextMenu->addAction(ui.actionExport_as_tea);
    contextMenu->addSeparator();
    contextMenu->addAction(ui.actionClose_route);
    contextMenu->addAction(ui.actionDelete_route_from_database);

    //ui.listWidgetResult->item(t.row())->setSelected(true); //select despite right click

    contextMenu->exec(ui.listWidget->mapToGlobal(pos));

}

void TEA::updatePath(QListWidgetItem *Item)
{
    PathList *Entry = dynamic_cast<PathList *>(Item);
    Entry->getPath()->setVisible( Entry->checkState() == Qt::Checked );
}

void TEA::centerMapOnSelectedRoute()
{   //TODO redundanz entfernen, siehe editMetadata (extra funkt.?)
    if( ui.listWidget->selectedItems().size() == 0 )
    {
	ui.textInformation->append(QString("No Item selected."));
	return;
    }
    QListWidgetItem *Item = ui.listWidget->selectedItems().first();
    PathList *Entry = dynamic_cast<PathList *>(Item);

    QSqlQuery adbquery(QSqlDatabase::database("adb"));
    adbquery.exec(qPrintable("Select * FROM active_metadata WHERE auid="+QString::number(Entry->getAuid())));
    adbquery.first();

    double latdiff = fabs(adbquery.record().value(13).asDouble() - adbquery.record().value(14).asDouble())/(PI*10000000);
    double londiff = fabs(adbquery.record().value(15).asDouble() - adbquery.record().value(16).asDouble())/(PI*10000000);
    double height = ui.graphicsView->mapToScene(0,0,ui.graphicsView->width(),ui.graphicsView->height()).boundingRect().height();
    double width = ui.graphicsView->mapToScene(0,0,ui.graphicsView->width(),ui.graphicsView->height()).boundingRect().width();
    int zoom = 0;
    if((latdiff < height) || (londiff < width))
    {
	do{
	    zoom++;
	    height = height / 2;
	    width = width / 2;
	}while((latdiff < height) || (londiff < width));
	zoom--;
    }
    else
    {
	do{
	    zoom--;
	    height = height * 2;
	    width = width * 2;
	}while((latdiff > height) || (londiff > width));
    }

    //center on selected Path
    ui.graphicsView->centerOn(dynamic_cast<QGraphicsItem *>(Entry->getPath()));
    //zoom and load tiles
    ui.sldZoom->setValue(zoom+zoomOld);
}

void TEA::editMetadata()
{
    if( ui.listWidget->selectedItems().size() == 0 )
    {
	ui.textInformation->append(QString("No route selected."));
	return;
    }
    QListWidgetItem *Item = ui.listWidget->selectedItems().first();
    PathList *Entry = dynamic_cast<PathList *>(Item);
    MetadataDialog d(QString::number(Entry->getAuid()), 1);
    d.exec();

    QSqlQuery adbquery(QSqlDatabase::database("adb"));
    adbquery.exec(qPrintable("Select * FROM active_metadata WHERE auid="+QString::number(Entry->getAuid())));
    adbquery.first();
    Entry->setText(adbquery.record().value(6).toString());
    adbquery.finish();
}

void TEA::actionViewDatabase()
{
DatabaseViewer d;
d.exec();
}

void TEA::updateADB()
{
    PathList *Entry = dynamic_cast<PathList *>(ui.listWidget->currentItem());
    if( Entry == 0 ) return;	//return if no Item is selected

    QString Name = Entry->text();
    Name.append("'").prepend("'");
    QSqlQuery adbquery(QSqlDatabase::database("adb"));
    adbquery.exec(qPrintable("UPDATE active_metadata SET name="+Name+" WHERE auid="+QString::number(Entry->getAuid())));

    adbquery.finish();
}

void TEA::setGeneralSettings()
{
    //TODO: Load from DB

    //manual
    QString address = ui.edtProxy->text();
    if (address != "") {
	QString ip = address.section(":",0,0);
	int port = address.section(":", -1).toInt();
	QNetworkProxy proxy = QNetworkProxy::QNetworkProxy(QNetworkProxy::HttpCachingProxy,ip,port);
	networkManager->setProxy(proxy);
    } else {
	QNetworkProxy proxy = QNetworkProxy::QNetworkProxy(QNetworkProxy::NoProxy);
	networkManager->setProxy(proxy);
    }

    //TODO: Save to DB
}

void TEA::trainerModeChanged()
{
	fillTrainerViewCBoxes();
}

void TEA::trainerSelectionChange()
{
	drawTrainer(ui.cboxX->currentIndex(),ui.cboxY->currentIndex());
}

void TEA::drawTrainer(int indexX, int indexY)
{
	//get auids

	QwtArray<double> x,y;
	int value,routeNum = 0; double factor = 1.0;

	QSqlQuery auidQuery = getCurrentlyLoadedRoutes();

	QwtPlotCurve *curve = new QwtPlotCurve;


	if (ui.rbNode->isChecked()){
		ui.qwtPlot->clear();

		switch (ui.cboxY->currentIndex()) {
			case 1: value = 6; ui.qwtPlot->setAxisTitle(0,"Altitude in m"); factor = 0.1; break;
			case 2:	value = 1; ui.qwtPlot->setAxisTitle(0,"Velocity in km/h"); factor = 0.01; break;
			case 3: value = 3; ui.qwtPlot->setAxisTitle(0,"Slope in deg"); factor = 0.1; break;
			case 4: value = 2; ui.qwtPlot->setAxisTitle(0,"Pedal frequency in RPM"); factor = 0.1; break;
			default: value = 1; ui.qwtPlot->setAxisTitle(0,"NYI"); factor = 0.0; break;
		}

		switch (ui.cboxX->currentIndex()) {
			case 0: ui.qwtPlot->setAxisTitle(2,"Time in s"); break;
			default: ui.qwtPlot->setAxisTitle(2,"NYI"); break;
		}

		while (auidQuery.next())
		{

			QString auid = auidQuery.record().value(0).toString();

			QSqlQuery route = getRouteData(auid, "adb");

			int i=0;
			//x.clear(); y.clear();
			while (route.next())
			{
				if ((route.record().value(value).toDouble() != 0.0) && value == 6)
			    {  x << (double)i; y << (factor * route.record().value(value).toDouble());} else if (value != 6){
				    x << (double)i; y << (factor * route.record().value(value).toDouble()); }
				i++;
			}

			//curveList.at(routeNum)->setData(x,y);
			//curveList.at(routeNum)->attach(ui.qwtPlot);
			//plotList.at(routeNum)->attach(ui.qwtPlot);


			curve->setData(x,y);
			//curve->setStyle()
			curve->attach(ui.qwtPlot);
			ui.qwtPlot->replot();



			//todo use largest and not last path diagram
			//trainerScene->setSceneRect(path.boundingRect());
			//ui.graphicsViewTrainer->fitInView(path.boundingRect(),Qt::IgnoreAspectRatio);
			routeNum++;

		}

	} else {
	    ui.qwtPlot->clear();
	    switch (ui.cboxY->currentIndex()) {
		    case 0: value = 19; ui.qwtPlot->setAxisTitle(0,"Mean velocity in km/h"); factor = 1; break;
		    case 1: value = 18; ui.qwtPlot->setAxisTitle(0,"Mean altitude in m"); factor = 0.1; break;
		    case 3: value = 10; ui.qwtPlot->setAxisTitle(0,"Altitude gain in m"); factor = 0.1; break;
		    case 4: value = 11; ui.qwtPlot->setAxisTitle(0,"Altitude loss in m"); factor = 0.1; break;
		    default: value = 19; ui.qwtPlot->setAxisTitle(0,"NYI"); factor = 0.0; break;
	    }

	    int value2, factor2;
	    switch (ui.cboxX->currentIndex()) {
		    case 2: value2 = 19; ui.qwtPlot->setAxisTitle(2,"Mean velocity in km/h"); factor2 = 1; break;
		    case 3: value2 = 18; ui.qwtPlot->setAxisTitle(2,"Mean altitude in m"); factor2 = 0.1; break;
		    case 4: value2 = 10; ui.qwtPlot->setAxisTitle(2,"Altitude gain in m"); factor2 = 0.1; break;
		    case 5: value2 = 11; ui.qwtPlot->setAxisTitle(2,"Altitude loss in m"); factor2 = 0.1; break;
		    default: value2 = 19; ui.qwtPlot->setAxisTitle(2,"NYI"); factor2 = 0.0; break;
	    }

	    QSqlQuery metadata = getAllMetadata("adb");

	    while (metadata.next()) {
	    x << (factor * metadata.value(value).toDouble());
	    y << (factor2 * metadata.value(value2).toDouble());
	    }

	    curve->setData(x,y);
	    curve->setStyle(QwtPlotCurve::Lines);
	    curve->attach(ui.qwtPlot);
	    ui.qwtPlot->replot();


	}

	//get relevant data either from route or from metadata
	//draw
}

void TEA::mapSourceChanged()
{
	if (ui.rbMapnik->isChecked()) mapSource = "http://tile.openstreetmap.org/";
	else mapSource = "http://andy.sandbox.cloudmade.com/tiles/cycle/";
	int i = 0;
	while (scene->items().size()>i)
	{
		if (round(scene->items().at(i)->zValue()) != 19 && round(scene->items().at(i)->zValue()) != 20) //routes are at these zvalues
			scene->removeItem(scene->items().at(i));
		else i++;
	}
	getTilesInRange();
	ui.textInformation->append("now using "+mapSource);
}

void TEA::consoleButtonTriggered()
{
	ui.dwBottom->setVisible(!ui.dwBottom->isVisible());
}

void TEA::consoleChanged()
{
	ui.consoleAction->setChecked(ui.dwBottom->isVisible());
}

void TEA::zoom(int steps)
{
	cout << "steps" << endl;
	if (((ui.sldZoom->value()+steps) <= 18) && ((ui.sldZoom->value()+steps) >= 0 ))
		ui.sldZoom->setValue(ui.sldZoom->value()+steps);
}

void TEA::zoomIn()
{
	if (ui.sldZoom->value()<18) ui.sldZoom->setValue(ui.sldZoom->value()+1);
}

void TEA::zoomOut()
{
	if (ui.sldZoom->value()>0) ui.sldZoom->setValue(ui.sldZoom->value()-1);
}

void TEA::sldChanged(int value)
{

	int zoomNew = ui.sldZoom->value();
	int dZoom = zoomNew-zoomOld;
	//QRectF oldSceneRect = scene->sceneRect();
	ui.graphicsView->scale(pow(2,(dZoom)),pow(2,(dZoom)));
	/*
	scene->setSceneRect(oldSceneRect.x()+oldSceneRect.width()*(0.5 - pow(2.0,-dZoom-1)),
						oldSceneRect.y()+oldSceneRect.height()*(0.5 - pow(2.0,-dZoom-1)),
						oldSceneRect.width()*pow(2.0,-dZoom),
						oldSceneRect.height()*pow(2.0,-dZoom));
						*/
	QRectF viewRect = ui.graphicsView->mapToScene(0,0,ui.graphicsView->width(),ui.graphicsView->height()).boundingRect();
	cout << "x,y,w,h: " << QString::number(viewRect.x()).toStdString() << " "
						<< QString::number(viewRect.y()).toStdString() << " "
						<< QString::number(viewRect.width()).toStdString() << " "
						<< QString::number(viewRect.height()).toStdString() << endl;

	int i = 0;
	while (scene->items().size()>i)
	{
		if (round(scene->items().at(i)->zValue()) > ui.sldZoom->value() &&
			round(scene->items().at(i)->zValue()) != 19 && round(scene->items().at(i)->zValue()) != 20) //routes are at these zvalues
			scene->removeItem(scene->items().at(i));
		else i++;

		/*//todo outline / stroke
		if (scene->items().at(i)->zValue() == 19 || scene->items().at(i)->zValue() == 20)
		{


			QGraphicsPathItem *pathItemStroke = new QGraphicsPathItem;
			QPen pen; pen.setStyle(Qt::SolidLine); pen.setColor(QColor::fromRgb(0xFF,0xFF,0xFF));
			QBrush brush; brush.setStyle(Qt::SolidPattern); brush.setColor(QColor::fromRgb(0xFF,0xFF,0xFF));
			QPainterPath pathStroke;
			QPainterPathStroker stroker; stroker.setWidth(0.1*pow(-2.0, zoomNew)); pathStroke = stroker.createStroke(path);

			pathItemStroke->setPath(pathStroke);
			pathItemStroke->setPen(pen);
			pathItemStroke->setBrush(brush);
			pathItemStroke->setZValue(19);

			scene->removeItem(scene->items().at(i));
			scene->addItem(pathItemStroke);


		}
		*/
	}


	getTilesInRange();
	zoomOld = ui.sldZoom->value();
}

TEA::~TEA()
{

}

void TEA::About()
{
    QMessageBox::about(this, tr("About TEA"),
	     tr("<b>TEA</b> is an application which is used for the evaluation "
		"and analysis of tracked routes in the TEA data format."));

}

void TEA::loadFromFile()
{

	QString TEAFilePath = QFileDialog::getOpenFileName(this, tr("Open TEA route file"),
			"C:/Users/jf/Desktop/BeLL/Programm/sample_routes",
			tr("TEA route files (*.tea)"));
	if (TEAFilePath != "")
	{
		ui.textInformation->append("Path chosen: " + TEAFilePath);
		ui.textInformation->append("Checksum: " + generateChecksumFromFile(TEAFilePath));

		if (!routePresentInDBs(generateChecksumFromFile(TEAFilePath)))
		{

			ui.textInformation->append("Route not yet loaded.\nLoading Route...");

			QString auid = importRoute(TEAFilePath);
			ui.textInformation->append("Route loaded. Requesting metadata...");
			qDebug("Requesting metadata");
			if(!getMetadata(auid))	//if the user rejected to load the file
			{
			    deleteRoute(auid, "adb");
			    return;
			}
			ui.textInformation->append("Metadata written.");
			qDebug("Metadata written");

			drawRoute(auid);

		} else ui.textInformation->append("Route seems to have already been loaded or saved.");

	}


}

bool TEA::nodeNextSkip(QSqlQuery routeData, int timesToSkip)
{
	int i;
	for(i=1; i<timesToSkip; i++) routeData.next();
	return routeData.next();
}

int TEA::getMetadata(QString auid)
{
	//load existing metadata
	MetadataDialog d(auid);
	return d.exec();
}

void TEA::saveToDatabase()
{
    //get selection
    QList<QListWidgetItem*> selectedItems = ui.listWidget->selectedItems();
qDebug("saveToDatabase() 1");
    for (int i=0; i<selectedItems.count(); i++)
    {
	//get pathlist
	qDebug("saveToDatabase() 2");
	PathList *Entry = dynamic_cast<PathList *>(selectedItems.at(i));
	qDebug("saveToDatabase() 3");

	//save all changes from adb to rdb
	if (Entry != 0) saveRoute(QString::number(Entry->getAuid())); //should work
	qDebug("saveToDatabase() 4");
	//TODO: might be useful: implement "getAllSelectedUIDs"
    }

	drawTrainer(ui.cboxX->currentIndex(),ui.cboxY->currentIndex());
}

void TEA::loadFromDatabase()
{
	FindDialog d;
	d.exec();
	drawRoutes(getCurrentlyLoadedRoutes());
	drawTrainer(ui.cboxX->currentIndex(),ui.cboxY->currentIndex());
}

void TEA::drawRoute(QString auid)
{
    QSqlQuery routeData = getRouteData(auid, "adb");
    QSqlRecord metadata = getRouteMetadata(auid, "adb");

    PathList *Entry = new PathList(metadata.value(6).toString(), auid.toInt());

    //TODO: check if item is already present in ItemView (+Path)
    if( ui.listWidget->currentIndex().row() != auid.toInt() )
    {
	Entry->setAuid( auid.toInt() );
	ui.listWidget->insertItem(auid.toInt(), Entry );
	ui.listWidget->setCurrentItem(Entry);
    }
    else    //this case should not occour theoretically, but just to be sure.
    {
	ui.textInformation->append(QString("Item has been already added"));
	return;
    }

    //remove all objects residing above the current zoom strata
    /*int i = 0;
    while (scene->items().size()>i)
    {
	if (round(scene->items().at(i)->zValue()) == 19 || round(scene->items().at(i)->zValue()) == 20)
	    scene->removeItem(scene->items().at(i));
	else i++;

    }*/



    //ui.listWidget->addItem(QString::number(auid.toInt()+1)+" - "+metadata.value(6).toString());

    int nodeSkips = metadata.value(20).toInt()/2500+1;
    QPainterPath path;

    routeData.first();
    prgBar->setMaximum(metadata.value(20).toInt());


    double x,y,tempX,tempY;
    //skip zeroes

    while ((getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString())) == 0) && (nodeNextSkip(routeData,0)));

    if (routeData.isValid()) {
	x=getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString()));
	y=getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString()));
	qDebug("First coordinate: "+QString::number(x)+"; "+QString::number(y));
	path.moveTo(x,y);
    }

    /* Draw Path */
    while (nodeNextSkip(routeData,nodeSkips))
    {
	prgBar->setValue(routeData.value(0).toInt());
	qApp->processEvents();

	tempX=getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString()));
	tempY=getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString()));
	//ui.textInformation->append(QString::number(tempX)+' '+QString::number(tempY));

	/* schöne Variante */
	if (nodeNextSkip(routeData,nodeSkips)) {

	    if (((tempY != 0.0) || (tempX != 0.0)) && ((getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString())) != 0) || (getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString())) != 0) ))  {
		x = tempX; y = tempY;
		path.quadTo(x,y,getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString())),getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString())));
		//qDebug(QString::number(x)+" "+QString::number(y)+" "+QString::number(getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString())))+","+QString::number(getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString()))));
	    }
	}
	/* schnelle Variante. TODO: Wahl per Menüoption
		if( ( tempX != 0 ) || ( tempY != 0) ) {
		    path.lineTo(tempX, tempY);
		} */
    }

    /*Add Path to the Scene */
    QGraphicsPathItem *pathItem = new QGraphicsPathItem;
    Entry->setPath(pathItem);
    pathItem->setPath(path);
    pathItem->setZValue(20);
    scene->addItem(pathItem);
    scene->setSceneRect(-PI,-PI,2*PI,2*PI);
    prgBar->reset();
    centerMapOnSelectedRoute();
}

void TEA::unloadRoute()
{

}

void TEA::removeRoute()
{

}

void TEA::drawRoutes(QSqlQuery auidQuery)
{
	ui.listWidget->clear();
	while (auidQuery.next())
	{
		drawRoute(auidQuery.record().value(0).toString());
	}
}

void TEA::closeEvent(QCloseEvent *event)
{
	if (maybeExit()) {
		closeDBs();
		event->accept();} else event->ignore();
}

bool TEA::maybeExit()
{
    //todo: check if there are realy unsaved changes
    QMessageBox::StandardButton
	    ret = QMessageBox::warning(this, tr("TEA exit dialog"),
				       tr("There may be unsaved changes!"),
				       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret ==QMessageBox::Save)
    {
	//TODO check if this makes sense
	//save every route from ADB in RDB
	QSqlQuery adbquery(QSqlDatabase::database("adb"));
	QString auid;
	adbquery.exec("SELECT * FROM active_metadata");	//get all active routes
	while(adbquery.next())
	{
	    auid=adbquery.record().value(0).toString();	//get uid from query
	    saveRoute(auid);	//save route
	}
	adbquery.finish();
	return true;
    }
    if (ret == QMessageBox::Discard) return true;
    else if (ret == QMessageBox::Cancel) return false;
    return false;
}
