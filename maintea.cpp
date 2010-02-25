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
#include "qwt_plot.h"


#define PI 3.1415926535897932384626433832795

using namespace std;

// OSM


TEA::TEA(QWidget *parent) :
	QMainWindow(parent)
{
	init = true;
	networkManager = new QNetworkAccessManager(this);
	connect(networkManager, SIGNAL(finished(QNetworkReply*)),
	         this, SLOT(downloaded(QNetworkReply*)));
	ui.setupUi(this);
	ui.textInformation->append("<b>TEA console</b>");
	scene = new QGraphicsScene;
	trainerScene = new QGraphicsScene;

	//tile size: 256px x 256px, in coordinates: 2*PI x 2*PI
	ui.graphicsView->scale(128/PI,128/PI);
	mapSource = "http://andy.sandbox.cloudmade.com/tiles/cycle/";
	getTile(0,0,0);
	//initially zoom always = 0
	zoomOld = 0;
	ui.graphicsView->setScene(scene);
	ui.graphicsViewTrainer->setScene(trainerScene);
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


	if (!initialiseDBs())
	{
	     QMessageBox::critical(this, tr("A critical error occured!"),
	                                     "A critical error occured while intialising active routes",
	                                     QMessageBox::Abort | QMessageBox::Ignore);
	}


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

void TEA::trainerZoom(int steps)
{
	ui.graphicsViewTrainer->scale(pow(2.0,steps),pow(2.0,steps));
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

		ui.cboxX->addItem("Time");
		ui.cboxX->addItem("Distance");
	} else {
	ui.cboxY->addItem("Mean velocity");
	ui.cboxY->addItem("Mean altitude");
	ui.cboxY->addItem("Duration");

	ui.cboxX->addItem("Date");
	ui.cboxX->addItem("Duration");
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
	if ((QString::fromAscii(tile)!="0") ) placeTile(tile, tileX, tileY, zoomLevel);
			else if (ui.sldZoom->value() == zoomLevel) networkManager->get(QNetworkRequest(QUrl(qPrintable(
					mapSource+QString::number(zoomLevel)+"/"+QString::number(tileX)+"/"+QString::number(tileY)+".png"))));
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

	placeTile(data, tileX, tileY, zoomLevel);

	//newTile->setPos(-PI, -PI);
	//newTile->scale(PI/128, PI/128);
	ui.textInformation->append("Tile image file size:" + QString::number(data.length()) + " bits (x:"+QString::number(tileX)+" y:"+QString::number(tileY)+" z:"+QString::number(zoomLevel));
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
	ui.tbMain->addAction(ui.saveAction);
	ui.tbMain->addAction(ui.saveAllAction);
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
	connect(ui.loadFromFileAction, SIGNAL(triggered()), this, SLOT(ActionLoadFromFile()));
	connect(ui.loadFromDatabaseAction, SIGNAL(triggered()), this, SLOT(ActionLoadFromDatabase()));
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
	connect(ui.graphicsViewTrainer, SIGNAL(wheelZoom(int)), this, SLOT(trainerZoom(int)));
	//connect(ui.graphicsView, SIGNAL(resizeEvent()), this, SLOT(graphicsViewResized()));
	//connect(ui.graphicsView, SIGNAL(mousePressed()), this, SLOT(grphPressed()));
	//ui.tbMain->addAction(QIcon("icons/32x32_0560/map.png"), "Something with maps", this, "mapAction");
}

void TEA::trainerModeChanged()
{
	trainerScene->clear();
	fillTrainerViewCBoxes();
}

void TEA::trainerSelectionChange()
{
	trainerScene->clear();
	drawTrainer(ui.cboxX->currentIndex(),ui.cboxY->currentIndex());
}

void TEA::drawTrainer(int indexX, int indexY)
{
	//get auids
	int value;
	switch (ui.cboxY->currentIndex()) {
		//case 0: todo:
		case 1: value = 6; break;
		case 2:	value = 1; break;
		case 3: value = 3; break;
		default: value = 1; break;
	}
	QSqlQuery auidQuery = getCurrentlyLoadedRoutes();
	QPen pen; pen.setColor(QColor::fromRgb(0,0,0,127));
	QGraphicsTextItem 	*x5,*x4,*x3,*x2,*x1,*x0,
						*y5,*y4,*y3,*y2,*y1,*y0	= new QGraphicsTextItem;
	trainerScene->setSceneRect(0,0,ui.graphicsViewTrainer->width(),ui.graphicsViewTrainer->height());


	if (ui.rbNode->isChecked()){

		while (auidQuery.next())
		{

			QString auid = auidQuery.record().value(0).toString();

			QSqlQuery route = getRouteData(auid, "adb");
			QPainterPath path; path.moveTo(0.0,0.0);
			QGraphicsPathItem *pathItem = new QGraphicsPathItem; pathItem->setPen(pen);

			int i=0;
			while (route.next() && route.next())
			{
				path.lineTo((double)i,-route.record().value(value).toDouble()/100);
				i++;
			}

			//pathItem->scale();
			pathItem->setPath(path);
			trainerScene->addItem(pathItem);


			//todo use largest and not last path diagram
			//trainerScene->setSceneRect(path.boundingRect());
			//ui.graphicsViewTrainer->fitInView(path.boundingRect(),Qt::IgnoreAspectRatio);

		}

	} else {




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
	QRectF oldSceneRect = scene->sceneRect();
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

void TEA::ActionLoadFromFile()
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

			getMetadata(auid);
			ui.textInformation->append("Metadata written.");

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

void TEA::getMetadata(QString auid)
{
	//load existing metadata
	MetadataDialog d(auid);
	d.exec();
}

void TEA::ActionSaveToDatabase()
{
	//get selection
	//save metadata
	drawTrainer(ui.cboxX->currentIndex(),ui.cboxY->currentIndex());
}

void TEA::ActionLoadFromDatabase()
{
	FindDialog d;
	d.exec();
	drawRoutes(getCurrentlyLoadedRoutes());
	drawTrainer(ui.cboxX->currentIndex(),ui.cboxY->currentIndex());
}

void TEA::drawRoute(QString auid)
{
        //remove all objects residing above the current zoom strata
	int i = 0;
	while (scene->items().size()>i)
	{
		if (round(scene->items().at(i)->zValue()) == 19 || round(scene->items().at(i)->zValue()) == 20)
			scene->removeItem(scene->items().at(i));
		else i++;

	}


	QSqlQuery routeData = getRouteData(auid, "adb");
	QSqlRecord metadata = getRouteMetadata(auid, "adb");

	ui.listWidget->addItem(QString::number(auid.toInt()+1)+" - "+metadata.value(6).toString());

	int nodeSkips = metadata.value(20).toInt()/2500+1;
	QPainterPath path;

	routeData.first();
	prgBar->setMaximum(metadata.value(20).toInt());
	path.moveTo(getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString())),
				getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString())));

	double x,y;
	while (nodeNextSkip(routeData,nodeSkips))
	{
		prgBar->setValue(routeData.value(0).toInt());
		qApp->processEvents();
		x=getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString()));
		y=getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString()));
		if (nodeNextSkip(routeData,nodeSkips)) path.quadTo(x,y,getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString())),getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString())));
	}

	QGraphicsPathItem *pathItem = new QGraphicsPathItem;
	pathItem->setPath(path);
	pathItem->setZValue(20);
	scene->addItem(pathItem);
	scene->setSceneRect(-PI,-PI,2*PI,2*PI);
	prgBar->reset();
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
	QMessageBox::StandardButton
	         ret = QMessageBox::warning(this, tr("TEA exit dialog"),
	                      tr("There may be unsaved changes!"),
	                      QMessageBox::Discard | QMessageBox::Cancel);
	         if (ret == QMessageBox::Discard)
	             return true;
	         else if (ret == QMessageBox::Cancel)
	             return false;
	         return false;
}
