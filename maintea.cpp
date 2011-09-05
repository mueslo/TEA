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
#include "activeroutelistitem.h"


#define PI 3.1415926535897932384626433832795

using namespace std;

//Most important TODO: Split maintea.cpp to make it less painful to search for a specific function

//TODO: Either *always* address routes by ActiveRouteListItem OR *always* by auid.

//Abbreviations:
// TODO: To do, something that needs to be done
// POI: Point of interest, something that might be worth implementing
// TAT: Think about this

//Multi-line comment syntax:
//First line, (paragraph) heading
// Second line
// Third line
//  subparagraph
// etc.
//New paragraph
// etc.

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

        currentRequests = 0;
	networkManager = new QNetworkAccessManager(this);
	connect(networkManager, SIGNAL(finished(QNetworkReply*)),
		 this, SLOT(downloaded(QNetworkReply*)));

        ui.setupUi(this);
	ui.textInformation->append("<b>TEA console</b>");
        scene = new QGraphicsScene(ui.graphicsView);
        ui.graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

	//tile size: 256px x 256px, in coordinates: 2*PI x 2*PI
	ui.graphicsView->scale(128/PI,128/PI);

        //todo: rewrite map source selection strategy to make it more elegant
	mapSource = "http://andy.sandbox.cloudmade.com/tiles/cycle/";

        getTile(0,0,0);

        //initially zoom always = 0
        zoomOld = 0;

        ui.graphicsView->setScene(scene);

        connectSignalsAndSlots();
	createToolBar();
	createStatusBar();
	fillTrainerViewCBoxes();
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
		ui.cboxY->setCurrentIndex(2);
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
        //TODO: extend this function so that the tiles are not requested from upper left to lower right
        // corner, but from the center outward.
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
        QGraphicsPixmapItem *tilePixmapItem = new QGraphicsPixmapItem(tilePixmap);
        //TAT: could the tiles create dangling/orphaned pointers? i thought so, but they don't seem to
        // because the error only occurs when you have loaded a route
	tilePixmapItem->setPos(tileToCoord(tileX,tileY,zoomLevel));
	tilePixmapItem->scale(PI / pow(2.0, 7 + zoomLevel),PI / pow(2.0, 7 + zoomLevel));
	tilePixmapItem->setZValue(zoomLevel);
	scene->addItem(tilePixmapItem);
}

void TEA::getTile(int tileX, int tileY, int zoomLevel)
{
    //TODO: check if present in DB
    QByteArray tile = getTileFromDB(zoomLevel,tileX,tileY,mapSource);
    //if ((QString::fromAscii(tile)!="0") ) placeTile(tile, tileX, tileY, zoomLevel);
    if (QString::fromAscii(tile)!="0"){
        placeTile(tile, tileX, tileY, zoomLevel);
    } else if (ui.sldZoom->value() == zoomLevel)
    {   QNetworkRequest request(QUrl(qPrintable(
                                         mapSource+QString::number(zoomLevel)+"/"+
                                         QString::number(tileX)+"/"+
                                         QString::number(tileY)+".png")));

        //POI: another possibility would be to save all requests in a variable of type
        // QStack<QQueue<QNetworkRequest>>. This way, the order in which the tiles are downloaded
        // and subsequently displayed could be kept, but the redundant downloading below would be a bit
        // less easy to implement.

        if (currentRequests<5)
        {
            //POI: a problem in the current system would be that the same data could be downloaded twice,
            // for example when one request is put in the stack then you zoom somewhere else, then you go back to the tile that is in the stack
            // and it is put in the stack again. I'm not sure if it is worth the computational effort here to check every time whether this
            // request is already in the request stack:
            // if (newTileRequest(request)) { ... }
            networkManager->get(request);
            ++currentRequests;
        } else {
        requestStack.push(request);
        }
        qDebug(QString::number(currentRequests)+" "+QString::number(requestStack.count()));
    }
}

void TEA::downloaded(QNetworkReply* reply)
{
    //NYI: limit maximum number of current downloads to 2 as specified in the CycleMap tileserver terms,
    //this would severely degrade download performance, though. Maybe keep limit at ~5?


    --currentRequests;
    if (!requestStack.isEmpty())
    {
    networkManager->get(requestStack.pop());
    ++currentRequests;
    }
    qDebug(QString::number(currentRequests)+" "+QString::number(requestStack.count()));

    QByteArray data = reply->readAll();
    QString path = reply->url().path();
    path.chop(4);
    QString noNetworkMessage("Tileserver is not reachable.");
    if (ui.rbCycleMap->isChecked()) path.remove(0,12);

    int zoomLevel = path.section('/',1,1).toInt();
    int tileX = path.section('/',2,2).toInt();
    int tileY = path.section('/',3,3).toInt();

    //actual data returned
    if (data.length()!=0) //check whether tile request returned data
    {
        addTileToDB(zoomLevel,tileX,tileY,data,mapSource); //this method also checks presence of tile in db
        ui.textInformation->append("Tile image file size:" + QString::number(data.length()) + " bits"
                                   "(x:"+QString::number(tileX)+" y:"+QString::number(tileY)+" z:"+QString::number(zoomLevel)+")");
    } //if warning message not yet displayed
    else if (!ui.textInformation->text().endsWith(noNetworkMessage)) ui.textInformation->append("<font color=red>Warning: </font>"+noNetworkMessage);

    if( ui.sldZoom->value() == zoomLevel ) placeTile(data, tileX, tileY, zoomLevel);

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

void TEA::exportSelected(QString format)
{
        for(int i=0;i<ui.lwActiveRoutes->selectedItems().count();++i)
        {
            ActiveRouteListItem *item = dynamic_cast<ActiveRouteListItem *>(ui.lwActiveRoutes->selectedItems().at(i));
            if (format.compare("kml",Qt::CaseInsensitive)==0) exportKML(item->getAuid());
            else qDebug("No such export format available.");
        }



}

void TEA::exportKML(QString auid)
{
    qDebug("exportKML");

    QString filePath = QFileDialog::getSaveFileName(this, tr("Export route as KML file"),
                                                       QDir::homePath(),
                                                       tr("Keyhole markup language files (*.kml)"));
    if (QFile::exists(filePath)) QFile::remove(filePath);
    if (filePath != "")
    {
        QFile templateFile("template.kml"); //TODO: introduce global variable of filename / relative directory to binary
        if (templateFile.exists())
        {
            if (templateFile.open(QIODevice::ReadOnly))
            {
                QFile file(filePath);
                if (file.open(QIODevice::WriteOnly | QIODevice::Append))
                {
                    QTextStream s(&file);
                    QSqlRecord metadata = getRouteMetadata(auid,"adb");
                    QSqlQuery routedata = getRouteData(auid,"adb");
                    QString name = metadata.value(6).toString();
                    /*
                    QString tags = metadata.value(5).toString();
                    QString location = metadata.value(4).toString();
                    QString date = metadata.value(2).toDate().toString();
                    QString duration = metadata.value(12).toTime().toString(); */

                    while (!templateFile.atEnd())
                    {
                        QString line = templateFile.readLine();
                        if (line.contains("__NAME__")) line.replace("__NAME__",name);
                        else if (line.contains("__COORDINATES__"))
                        {
                            QString coordinates;
                            QString lat;
                            QString lon;
                            QString alt;

                            //could be that first node is left out, check it

                            int max = metadata.value(20).toInt();
                            prgBar->setMaximum(100);
                            int i = 0;

                            while (routedata.next())
                            {
                                if (i%(max/100)==0) prgBar->setValue(++i);  //would lead to error for no entries but ten routedate.next() should never return anything
                                prgBar->setValue(++i);
                                //todo: progress
                                lat = QString::number(getLatFromRawLat(routedata.value(4).toString()));
                                lon = QString::number(getLonFromRawLon(routedata.value(5).toString()));
                                alt = QString::number(routedata.value(6).toInt()/10.0);
                                coordinates.append(lon+","+lat+","+alt+"\r\n");
                                //todo: tab spacing
                            }
                            line.replace("__COORDINATES__",coordinates);
                        }
                        s << line;
                    }
                    prgBar->reset();
                    ui.textInformation->append("Route successfully exported to "+filePath);
                }

                file.close();
                templateFile.close();
            }
        } else qDebug("KML template missing.");

    }


}

void TEA::exportTEA(QString auid)
{
    qDebug("exportTEA");
}

/* Uninteresting functions */
void TEA::exportSelectedKML() {exportSelected("kml");}
void TEA::exportSelectedTEA() {exportSelected("tea");}



/* */

void TEA::connectSignalsAndSlots()
{
    //TODO group this for easy viewing
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
        connect(ui.kmlExportAction, SIGNAL(triggered()), this, SLOT(exportSelectedKML()));
        connect(ui.actionExport_as_tea, SIGNAL(triggered()),this,SLOT(exportSelectedTEA()));
	connect(ui.cboxX, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerSelectionChange()));
	connect(ui.cboxY, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerSelectionChange()));
	connect(ui.rbNode, SIGNAL(toggled(bool)), this, SLOT(trainerModeChanged()));

	connect(ui.graphicsView, SIGNAL(wheelZoom(int)), this, SLOT(zoom(int)));
        connect(ui.graphicsView, SIGNAL(viewChanged()), this, SLOT(viewChange()));

	connect(ui.btnGeneralSettings, SIGNAL(clicked()), this, SLOT(setGeneralSettings()));
	connect(ui.databaseViewAction, SIGNAL(triggered()), this, SLOT(actionViewDatabase()));
	connect(ui.lwActiveRoutes, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(updatePath(QListWidgetItem*)));
	//connect(ui.saveAction, SIGNAL(triggered()), this, SLOT(UpdateADB()) );
	connect(ui.saveAction, SIGNAL(triggered()), this, SLOT(saveSelectedToDatabase()));
	connect(ui.actionEdit_metadata, SIGNAL(triggered()), this, SLOT(editMetadata()));
	connect(ui.actionCenter_Map, SIGNAL(triggered()), this, SLOT(centerMapOnSelectedRoute()));
	connect(ui.lwActiveRoutes, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showListContextMenu(const QPoint &)));
	connect(ui.saveAllAction, SIGNAL(triggered()), this, SLOT(saveAllToDatabase()));
        connect(ui.actionClose_route, SIGNAL(triggered()), this, SLOT(unloadSelected()));
	//connect(ui.graphicsView, SIGNAL(resizeEvent()), this, SLOT(graphicsViewResized()));
	//connect(ui.graphicsView, SIGNAL(mousePressed()), this, SLOT(grphPressed()));
	//ui.tbMain->addAction(QIcon("icons/32x32_0560/map.png"), "Something with maps", this, "mapAction");
}


void TEA::showListContextMenu(const QPoint &pos)
{
    QMenu *contextMenu = new QMenu(ui.lwActiveRoutes);
    QModelIndex t = ui.lwActiveRoutes->indexAt(pos);
    contextMenu->addAction(ui.saveAction);
    contextMenu->addAction(ui.actionCenter_Map);
    contextMenu->addAction(ui.actionEdit_metadata);
    contextMenu->addSeparator();
    contextMenu->addAction(ui.actionExport_as_tea);
    contextMenu->addAction(ui.kmlExportAction);
    contextMenu->addSeparator();
    contextMenu->addAction(ui.actionClose_route);
    contextMenu->addAction(ui.actionDelete_route_from_database);

    //ui.lwActiveRoutesResult->item(t.row())->setSelected(true); //select despite right click

    contextMenu->exec(ui.lwActiveRoutes->mapToGlobal(pos));

}

void TEA::updatePath(QListWidgetItem *Item)
{
    qDebug("updatePath");

    ActiveRouteListItem *ListItem = dynamic_cast<ActiveRouteListItem *>(Item);

    if(ListItem == 0) {qDebug("listitem is null"); return; }  //check if ListItem is actually valid

    QGraphicsPathItem *path = ListItem->getPath();
    QGraphicsPathItem *pathOutline = ListItem->getPathOutline();

    if (path != 0 && pathOutline != 0) //check if paths have not yet been set
    {
    path->setVisible( ListItem->checkState() == Qt::Checked );
    pathOutline->setVisible(ListItem->checkState()==Qt::Checked);
    } else qDebug("updatePath path or pathoutline null");

    QwtPlotCurve *curve = ListItem->getCurve();

    if (curve == 0) {qDebug("updatePath curve null"); return;} //check if curve has not yet been set
    qDebug("All routes and paths non-null.");
    if(ListItem->checkState()==Qt::Checked)
    {
        curve->attach(ui.qwtPlot);
    }
    else
    {
        curve->detach();
    }
    ui.qwtPlot->replot();
}

void TEA::centerMapOnSelectedRoute()
{   //TODO redundanz entfernen, siehe editMetadata (extra funkt.?)
    if( ui.lwActiveRoutes->selectedItems().size() == 0 )
    {
	ui.textInformation->append(QString("No Item selected."));
	return;
    }
    QListWidgetItem *Item = ui.lwActiveRoutes->selectedItems().first();
    ActiveRouteListItem *Entry = dynamic_cast<ActiveRouteListItem *>(Item);

    //if the item has no path data, abort
    if(Entry->getPath()->boundingRect().width() == 0.0)
    {
        ui.textInformation->append("This Item has no valid path");
	return;
    }

    QSqlRecord adbrecord=getRouteMetadata(Entry->getAuid(),"adb");//QSqlDatabase::database("adb"));
   // adbquery.exec(qPrintable("Select * FROM active_metadata WHERE auid="+QString::number(Entry->getAuid())));
   // adbquery.first();

    //TODO: implement nicer version of below

    double latdiff = fabs(adbrecord.value(13).asDouble() - adbrecord.value(14).asDouble())/(PI*10000000);
    double londiff = fabs(adbrecord.value(15).asDouble() - adbrecord.value(16).asDouble())/(PI*10000000);
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
    ui.tabwCentre->setCurrentIndex(0);
    if (zoom==0) getTilesInRange();     //why? getTiles is called when the zoom changes,
                                        //when it doesn't change (zoom==0), the tiles should already be loaded
}

void TEA::editMetadata()
{
    if( ui.lwActiveRoutes->selectedItems().size() == 0 )
    {
	ui.textInformation->append(QString("No route selected."));
	return;
    }
    QListWidgetItem *Item = ui.lwActiveRoutes->selectedItems().first();
    ActiveRouteListItem *Entry = dynamic_cast<ActiveRouteListItem *>(Item);
    MetadataDialog d(Entry->getAuid(), 1);
    d.exec();

    QSqlRecord adbrecord=getRouteMetadata(Entry->getAuid(),"adb");
    Entry->setName(adbrecord.value(6).toString());
    Entry->setModified();
    //adbquery.finish();
}

void TEA::actionViewDatabase()
{
DatabaseViewer d;
d.exec();
}

void TEA::updateADB()
{
    ActiveRouteListItem *Entry = dynamic_cast<ActiveRouteListItem *>(ui.lwActiveRoutes->currentItem());
    if( Entry == 0 ) return;	//return if no Item is selected

    QString Name = Entry->text();
    Name.append("'").prepend("'");
    QSqlQuery adbquery(QSqlDatabase::database("adb"));
    adbquery.exec(qPrintable("UPDATE active_metadata SET name="+Name+" WHERE auid="+Entry->getAuid()));

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
        QNetworkProxy proxy = QNetworkProxy(QNetworkProxy::HttpCachingProxy,ip,port);
	networkManager->setProxy(proxy);
    } else {
        QNetworkProxy proxy = QNetworkProxy(QNetworkProxy::NoProxy);
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
        redrawTrainer();
}

void TEA::redrawTrainer()
{
        //todo: implement route comparison mode
	//get auids

	int value,routeNum = 0; double factor = 1.0;

        QSqlQuery allMetadata = getCurrentlyLoadedRoutes();
        QString auid;
/*
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
*/
                while (allMetadata.next())
                    {
                        QSqlRecord* metadata = new QSqlRecord(allMetadata.record());
                        //TAT: move delete curve here from addcurve
                        auid = metadata->value(0).toString();

                        QSqlQuery *routedata = new QSqlQuery(getRouteData(auid, "adb"));
                        addCurve(find(auid),routedata,metadata);


                        delete routedata;
                        delete metadata;

                        /*
                        QwtArray<double> x,y;
                        QwtPlotCurve *curve = new QwtPlotCurve; //new

			QString auid = auidQuery.record().value(0).toString();

                        //QND search for auid in listwidget to obtain row/Item
                        int row = -1;
                        ActiveRouteListItem *ListItem = 0;

                        for (int i = 0; i<(ui.lwActiveRoutes->count());++i)
                        {
                            QListWidgetItem *Item = ui.lwActiveRoutes->item(i);
                            ActiveRouteListItem *ListItemIt = dynamic_cast<ActiveRouteListItem *>(Item);
                            if (ListItemIt->getAuid()==auid) {row = i; ListItem = ListItemIt;}
                        }
			if( row == -1)
			{
			    qDebug("Kurve konnte nicht erstellt werden, keinen passenden ListItemEintrag gefunden.");
			    continue;
			}

                        //Item = ui.lwActiveRoutes->item(auid.toInt()); //AUID should match position of Item.
                        //ActiveRouteListItem *ListItem = dynamic_cast<ActiveRouteListItem *>(Item);


			QSqlQuery route = getRouteData(auid, "adb");

			int i=0;
                        x.clear(); y.clear();
                        //iterate over all nodes
			while (route.next())
                        {

                            if ((route.record().value(value).toDouble() != 0.0) && value == 6)
                                {  x << (double)i; y << (factor * route.record().value(value).toDouble());}
                                else if (value != 6){
				    x << (double)i; y << (factor * route.record().value(value).toDouble()); }
                            i++;

			}

			//curveList.at(routeNum)->setData(x,y);
			//curveList.at(routeNum)->attach(ui.qwtPlot);
			//plotList.at(routeNum)->attach(ui.qwtPlot);
			curve->setData(x,y);

                        //curve->setBrush(Qt::cyan); //fill to baseline with QBrush

			//curve->setCurveAttribute(QwtPlotCurve::Fitted);	//THINK ABOUT
			curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);

                        //both lead to crashes on loading second route, why?
                        //maybe because it emits a signal and updatePath is called

			ListItem->setCurve(curve);
			if( ListItem->checkState() == Qt::Checked)
			{
                            ListItem->getCurve()->attach(ui.qwtPlot);
			}

			ui.qwtPlot->replot();


			//todo use largest and not last path diagram
			//trainerScene->setSceneRect(path.boundingRect());
			//ui.graphicsViewTrainer->fitInView(path.boundingRect(),Qt::IgnoreAspectRatio);
			routeNum++;
                        */
                    //}//new

                    }

/*
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
            QwtPlotCurve *curve = new QwtPlotCurve; //new

            QwtArray<double> x,y;
	    while (metadata.next()) {
	    x << (factor * metadata.value(value).toDouble());
	    y << (factor2 * metadata.value(value2).toDouble());
	    }

	    curve->setData(x,y);
            curve->setStyle(QwtPlotCurve::Dots);
	    curve->attach(ui.qwtPlot);
	    ui.qwtPlot->replot();


	}

	//get relevant data either from route or from metadata
	//draw
        */
}

ActiveRouteListItem* TEA::find(QString auid)
{
    ActiveRouteListItem* route = 0;
    QList<QListWidgetItem*> allItems = ui.lwActiveRoutes->findItems("",Qt::MatchContains);
    int i = 0;
    while (i<allItems.count() && route==0)
    {
        ActiveRouteListItem* item = dynamic_cast<ActiveRouteListItem*>(allItems.at(i));
        if (item->getAuid()==auid) route=item;
        ++i;
    }
    return route;
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
        /*
        cout << "x,y,w,h: " << QString::number(viewRect.x()).toStdString() << " "
						<< QString::number(viewRect.y()).toStdString() << " "
						<< QString::number(viewRect.width()).toStdString() << " "
						<< QString::number(viewRect.height()).toStdString() << endl;
        */
	int i = 0;
        //Iterate through items and delete it if it is in the wrong zoom layer, if not, add 1 to counter.
        //The next time in the while loop it will skip the items already in the correct zoom layer.
        //Once the number of items is equal to the number of skips, all offending tiles have been deleted.

        //todo: unload all tiles (that are not in new zoom layer), always get tiles from the database

	while (scene->items().size()>i)
        {
            //ZLevel 19: Path Outlines, ZLevel 20: Paths
            //limiting the number of layers that are above each other at any given moment
            // increases performance (at least on linux) extremely
            //TODO: implement that if tiles at a given zoom layer are not available,
            // the ones in the zoom layer above will be loaded, current: workaround, 4 layers are shown
            // maybe you should be able to choose maxLayers as a sort of performance setting
            // another possibility would be to have one 'master' background which always gets the last recent
            // map data and only the newest tiles are displayed
            int z = round(scene->items().at(i)->zValue());
            int maxLayers = 4;
                if ((z > ui.sldZoom->value() || z<ui.sldZoom->value()-(maxLayers-1)) &&
                        z != 19 && z != 20) //routes are at these zvalues
			scene->removeItem(scene->items().at(i));
                else i++;
	}

        for (int i = 0; i<(ui.lwActiveRoutes->count());++i)
        {
            QListWidgetItem *Item = ui.lwActiveRoutes->item(i);
            ActiveRouteListItem *ListItem = dynamic_cast<ActiveRouteListItem *>(Item);
            ListItem->setOutlineZoom(zoomNew);
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
                        QDir::homePath(),
			tr("TEA route files (*.tea)"));
        if (TEAFilePath != "" && QFile::exists(TEAFilePath))
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

                        addRoute(auid, true);

		} else ui.textInformation->append("Route seems to have already been loaded or saved.");

	}


}

bool TEA::nodeNextSkip(QSqlQuery routeData, int timesToSkip)
{
	int i;
	for(i=1; i<timesToSkip; i++) routeData.next();
	return routeData.next();
}

bool TEA::nodeNextSkip(QSqlQuery *routeData, int timesToSkip)
{
        int i;
        for(i=1; i<timesToSkip; i++) routeData->next();
        return routeData->next();
}

int TEA::getMetadata(QString auid)
{
	//load existing metadata
	MetadataDialog d(auid);
	return d.exec();
}

void TEA::saveSelectedToDatabase()
{
    //get selection
    QList<QListWidgetItem*> selectedItems = ui.lwActiveRoutes->selectedItems();
    saveToDatabase(selectedItems);
}

void TEA::saveToDatabase(QList<QListWidgetItem*> chosenItems)
{

    if(chosenItems.isEmpty())
    {
	ui.textInformation->append(tr("No Item selected"));
	return;
    }
    for (int i=0; i<chosenItems.count(); i++)
    {
	//get ActiveRouteListItem
	ActiveRouteListItem *Entry = dynamic_cast<ActiveRouteListItem *>(chosenItems.at(i));

	//save all changes from adb to rdb
	if (Entry != 0)
	{
            QString next_uid = saveRoute(Entry->getAuid()); //save occurs here
            if(!next_uid.isEmpty()) //should work
	    {
		QSqlQuery adbquery(QSqlDatabase::database("adb"));
                adbquery.exec(qPrintable("UPDATE active_metadata SET uid="+next_uid+" WHERE auid="+Entry->getAuid()));
		Entry->setModified(0);
	    }

	}
	//TODO: might be useful: implement "getAllSelectedUIDs"
    }
}

void TEA::saveAllToDatabase()
{
    QList<QListWidgetItem*> allItems = ui.lwActiveRoutes->findItems("", Qt::MatchContains);
    saveToDatabase(allItems);
}

void TEA::loadFromDatabase()
{
	FindDialog d;
	d.exec();
        addNewRoutes(getCurrentlyLoadedRoutes(),false);
}

//TODO: addRoute to replace drawRoute, addRoute does necessary modifications to the ListWidget,
// and calls drawMap(drawRoute) and drawTrainer

void TEA::addRoute(QString auid, bool modified)
{
    qDebug("addRoute");
    QSqlQuery *routedata = new QSqlQuery(getRouteData(auid, "adb"));
    QSqlRecord *metadata = new QSqlRecord(getRouteMetadata(auid, "adb"));
    ActiveRouteListItem *route = new ActiveRouteListItem(metadata->value(6).toString(), auid.toInt(), modified, ui.lwActiveRoutes);

    ui.lwActiveRoutes->selectionModel()->clearSelection();
    ui.lwActiveRoutes->insertItem(auid.toInt(), route ); //maybe just additem?
    ui.lwActiveRoutes->setCurrentItem(route);

    addPath(route,routedata,metadata);
    addCurve(route,routedata,metadata);

    updatePath(route);

    delete routedata;
    delete metadata;
}

void TEA::addCurve(ActiveRouteListItem *route, QSqlQuery *routedata, QSqlRecord *metadata)
{
    qDebug("Generating curve.");

    //todo: implement route comparison mode
    //todo: check if new trainer value differs from old one

    if (ui.rbNode->isChecked()){
        qDebug("Node comparison");
        int value = 0; double factor = 1.0;

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

        QwtArray<double> x,y;
        if (route->getCurve()!=0) {qDebug("Deleting old curve"); route->getCurve()->detach(); delete route->getCurve();}

        QwtPlotCurve *curve = new QwtPlotCurve;
        int i=0;
        x.clear(); y.clear();
        //iterate over all nodes
        while (routedata->next())
        {
            if (((routedata->record().value(value).toDouble() != 0.0) && value==6) || value != 6)
            {  x << (double)i; y << (factor * routedata->record().value(value).toDouble());}
            i++;

        }

        curve->setData(x,y);

        //curve->setBrush(Qt::cyan); //fill to baseline with QBrush

        //curve->setCurveAttribute(QwtPlotCurve::Fitted);	//THINK ABOUT
        curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);

        route->setCurve(curve);
        if(route->checkState() == Qt::Checked)
        {
            route->getCurve()->attach(ui.qwtPlot);
        }

        ui.qwtPlot->replot();
    } else qDebug("Route comparison not yet implemented");
    routedata->first();

}

void TEA::addPath(ActiveRouteListItem *route, QSqlQuery *routedata, QSqlRecord *metadata)
{
    qDebug("Generating path.");
    int nodeSkips = metadata->value(20).toInt()/2500+1;
    QPainterPath path;

    routedata->first();
    prgBar->setMaximum(metadata->value(20).toInt());

    double x,y,tempX,tempY;
    //skip zeroes

    while ((getMercatorYFromLat(getLatFromRawLat(routedata->value(4).toString())) == 0) && (nodeNextSkip(routedata,0)));

    if (routedata->isValid()) {
        x=getMercatorXFromLon(getLonFromRawLon(routedata->value(5).toString()));
        y=getMercatorYFromLat(getLatFromRawLat(routedata->value(4).toString()));
	qDebug("First coordinate: "+QString::number(x)+"; "+QString::number(y));
        path.moveTo(x,y);
    }

    /* Draw Path */
    while (nodeNextSkip(routedata,nodeSkips))
    {
        prgBar->setValue(routedata->value(0).toInt());
	qApp->processEvents();

        tempX=getMercatorXFromLon(getLonFromRawLon(routedata->value(5).toString()));
        tempY=getMercatorYFromLat(getLatFromRawLat(routedata->value(4).toString()));
	//ui.textInformation->append(QString::number(tempX)+' '+QString::number(tempY));

	/* schöne Variante
	if (nodeNextSkip(routeData,nodeSkips)) {

	    if (((tempY != 0.0) || (tempX != 0.0)) && ((getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString())) != 0) || (getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString())) != 0) ))  {
		x = tempX; y = tempY;
		path.quadTo(x,y,getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString())),getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString())));
		//qDebug(QString::number(x)+" "+QString::number(y)+" "+QString::number(getMercatorXFromLon(getLonFromRawLon(routeData.value(5).toString())))+","+QString::number(getMercatorYFromLat(getLatFromRawLat(routeData.value(4).toString()))));
	    }
	}*/
        /* schnelle Variante */
		if( ( tempX != 0 ) || ( tempY != 0) ) {
                    path.lineTo(tempX, tempY);
		}
    }

    /*Add Path and Outline to the Scene */
    QGraphicsPathItem *pathItem = new QGraphicsPathItem;
    QGraphicsPathItem *pathItemOutline = new QGraphicsPathItem;

    route->setPath(pathItem);
    route->setPathOutline(pathItemOutline);
    route->setOutlineZoom(ui.sldZoom->value()); //to get outline
    pathItem->setPath(path);
    pathItemOutline->setPath(path);

    pathItem->setZValue(20);
    pathItemOutline->setZValue(19);
    pathItemOutline->setOpacity(0.7);

    scene->addItem(route->getPath());
    scene->addItem(route->getPathOutline());
    scene->setSceneRect(-PI,-PI,2*PI,2*PI);

    ui.lwActiveRoutes->setCurrentItem(route);
    centerMapOnSelectedRoute();
    //TODO: implement centerMapOnRoute(QString auid);

    prgBar->reset();
    routedata->first();
}

bool TEA::save(QList<QListWidgetItem*> routes, TEA::Save behaviour)
{
    if (routesModified())
    {

        if(routes.isEmpty())
        {
            ui.textInformation->append(tr("No Item selected"));
            return true;
        }

        TEA::Save newBehaviour;
        if (routes.count() != 1) {
            if (behaviour != TEA::AskToSave) newBehaviour=behaviour;
            else
            {
                QMessageBox saveModeBox(QMessageBox::Warning,"Closing routes", "There are unsaved changes in your routes.");
                QPushButton *SaveAllButton = saveModeBox.addButton(QMessageBox::SaveAll);
                QPushButton *PromptForEachButton = saveModeBox.addButton(tr("Prompt for each"), QMessageBox::AcceptRole);
                QPushButton *DiscardAllButton = saveModeBox.addButton(tr("Discard all"), QMessageBox::DestructiveRole);
                QPushButton *CancelButton = saveModeBox.addButton(tr("Cancel"), QMessageBox::RejectRole);

                saveModeBox.exec();

                if (saveModeBox.clickedButton() == SaveAllButton) newBehaviour = TEA::ForceSave;
                else if (saveModeBox.clickedButton() == PromptForEachButton) newBehaviour = TEA::AskToSave;
                else if (saveModeBox.clickedButton() == DiscardAllButton) newBehaviour = TEA::DoNotSave;
                else if (saveModeBox.clickedButton() == CancelButton) return false;
            }
        } else newBehaviour = behaviour;

        //for (int i=0; i<selectedItems.count(); i++)
        //for (int i=routes.count()-1; i>-1; i--)
        int i = routes.count()-1;
        bool accepted = true;
        while (i>-1 && accepted == true)
        {
            //get ActiveRouteListItem
            ActiveRouteListItem *route = dynamic_cast<ActiveRouteListItem *>(routes.at(i));
            accepted = save(route,newBehaviour);
            i--;
        }
        return accepted;
    } else return true;
}

bool TEA::save(ActiveRouteListItem *route, TEA::Save behaviour)
{
    //save changes
    QMessageBox::StandardButton ret;

    if(route->isModified() && behaviour==TEA::AskToSave)
    {

            ret = QMessageBox::warning(this, tr("Closing route "+route->getName()),
                                       tr("There are unsaved changes in your route "+route->getName()+"!"),
                                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    }
                if (ret==QMessageBox::Save || behaviour == TEA::ForceSave) saveRoute(route->getAuid());
                //uninitialised ret is always != QMessageBox::Cancel
                if (ret != QMessageBox::Cancel) return true; else return false;
}

void TEA::unload(ActiveRouteListItem *route)
{
    if (route != 0)
    {
        //routendaten löschen
        //aus active_metadata löschen
        route->getCurve()->detach();
        deleteRoute(route->getAuid(), "adb");

        //pfad löschen
        //pfadumrandung löschen
        //kurve löschen
        //eintrag löschen
        delete route->getCurve();
        delete route->getPath();
        delete route->getPathOutline();
        delete route;

        ui.qwtPlot->replot();

    }
    else
    {
        qDebug("Route is null.");
    }
}

void TEA::unloadSelected()
{

    QList<QListWidgetItem*> selectedItems = ui.lwActiveRoutes->selectedItems();
    if (save(selectedItems)) unload(selectedItems);
}

void TEA::unloadAll()
{
    QList<QListWidgetItem*> allItems = ui.lwActiveRoutes->findItems("",Qt::MatchContains);
    if (save(allItems)) unload(allItems);
}

void TEA::unload(QList<QListWidgetItem*> routes)
{
    //item ausfindig machen
    if(routes.isEmpty())
    {
	ui.textInformation->append(tr("No Item selected"));
	return;
    }
    //for (int i=0; i<selectedItems.count(); i++)
    for (int i=routes.count()-1; i>-1; i--)
    {
        ActiveRouteListItem *route = dynamic_cast<ActiveRouteListItem *>(routes.at(i));
        unload(route);
    }
}

void TEA::removeRoute()
{
    ;
}

bool TEA::routeAdded(QString auid)
{
    return (find(auid) != 0);
}

void TEA::addNewRoutes(QSqlQuery auidQuery, bool modified)
{
    qDebug("Adding new routes.");
    QString auid;
    while (auidQuery.next())
    {
        auid = auidQuery.value(0).toString();
        qDebug("Auid: "+auid);
        if (!routeAdded(auid)) {qDebug("Adding new route "+auid); addRoute(auid, modified);}
    }
}

void TEA::closeEvent(QCloseEvent *event)
{
	if (maybeExit()) {
		closeDBs();
		event->accept();} else event->ignore();
}

bool TEA::routesModified()
{
    QList<QListWidgetItem*> allItems = ui.lwActiveRoutes->findItems("", Qt::MatchContains);
    if(!allItems.isEmpty())
    {
        for (int i=0; i<allItems.count(); i++)
        {
            //get ActiveRouteListItem
            ActiveRouteListItem *Entry = dynamic_cast<ActiveRouteListItem *>(allItems.at(i));

            //save all changes from adb to rdb
            if (Entry != 0)
            {
                if(Entry->isModified()) return true;
            }
        }
        return false;
    }
}

bool TEA::maybeExit()
{
    QList<QListWidgetItem*> allItems = ui.lwActiveRoutes->findItems("", Qt::MatchContains);
    return save(allItems);
    /*
    QList<QListWidgetItem*> allItems = ui.lwActiveRoutes->findItems("", Qt::MatchContains);
    bool modified = routesModified();
    QMessageBox::StandardButton ret;
    if(modified)
    {
	    ret = QMessageBox::warning(this, tr("TEA exit dialog"),
				       tr("There are unsaved changes!"),
                                       QMessageBox::SaveAll | QMessageBox::Discard | QMessageBox::Cancel);
	}
    else
    {
	ret = QMessageBox::warning(this, tr("TEA exit dialog"),
				   tr("Do you really want to quit?"),
				   QMessageBox::Yes | QMessageBox::Cancel);
    }
    if (ret ==QMessageBox::SaveAll)
    {
	//TODO check if this makes sense
	//save every route from ADB in RDB

        //Falki: use 'saveAllToDatabase'
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
    if ((ret == QMessageBox::Discard) || (ret == QMessageBox::Yes)) return true;
    else if (ret == QMessageBox::Cancel) return false;

    return false;
    */
}
