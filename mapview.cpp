#include <mapview.h>
#define PI 3.1415926535897932384626433832795

OSMTile::OSMTile(QPixmap & pixmap, const QPoint & coords, int z) : QGraphicsPixmapItem(pixmap), Coordinates(coords)
{
    setPos(tileToCoord(coords.x(),coords.y(),z));
    scale(PI / pow(2.0, 7 + z),PI / pow(2.0, 7 + z));
    setZValue(z);
}

QPoint OSMTile::coords() const
{
    return Coordinates;
}

MapView::MapView(QWidget *parent) : QGraphicsView(parent)
{
    outlinePen.setBrush(Qt::white);
    Zoomable = 1;

    //NYI: limit maximum number of current downloads to 2 as specified in the CycleMap tileserver terms,
    // this would severely degrade download performance, though. Maybe keep limit at ~5?
    currentRequests = 0;
    requestLimit = 5;
    networkManager = new QNetworkAccessManager(this);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setScene(new QGraphicsScene(this)); //no delete necessary, parent set
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    //tile size: 256px x 256px, in coordinates: 2*PI x 2*PI
    scale(128/PI,128/PI); //default zoom == 0
    setSceneRect(-PI, -PI, 2*PI, 2*PI);

    mapSources.insert(OCM,{OCM,"OpenCycleMap", "http://tile.opencyclemap.org/cycle/", 0, 17});
    mapSources.insert(Mapnik,{Mapnik,"OpenStreetMap Mapnik", "http://tile.openstreetmap.org/", 0,18});
    mapSources.insert(None,{None,"None","",0,20});

    MapSource = {None, "None", "", 0 , 20};
    currentZoom = 0; zoomTo(MapSource.minzoom);

    connectSignalsAndSlots();
}

MapView::~MapView()
{

}

void MapView::connectSignalsAndSlots()
{
    connect(networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(tileDownloaded(QNetworkReply*)));

}


void MapView::tileDownloaded(QNetworkReply *reply)
{
    --currentRequests;
    if (!requestStack.isEmpty())
    {
        networkManager->get(requestStack.pop());
        ++currentRequests;
    }
    qDebug(QString::number(currentRequests)+" "+QString::number(requestStack.count()));

    QByteArray data = reply->readAll();
    QString path = reply->url().toString();

    int *x = getXYZFromUrl(path);
    allRequests.removeAll(QVector3D(x[0],x[1],x[2]));
    qDebug("allrequests size"+QString::number(allRequests.size()));

    path.remove(QString::number(x[2])+"/"+QString::number(x[0])+"/"+QString::number(x[1])+path.right(4));

    //actual data returned
    if (data.length()!=0) //check whether tile request returned valid data, TODO: check if >= arbitrary value
    {
        addTileToDB(x[2],x[0],x[1],data,path); //this method also checks presence of tile in db
        //qDebug(qPrintable("Tile image file size:" + QString::number(data.length()) + " bits"
        //                           "(x:"+QString::number(x)+" y:"+QString::number(y)+" z:"+QString::number(z)+")"));
    }
    else emit noNetwork();

    //check if zoomlevel and map source did not change while downloading
    if( currentZoom == x[2] && path == MapSource.url) placeTile(data, x[0], x[1], x[2]);
    delete[] x;
    reply->deleteLater(); //do not 'delete reply;'
}

void MapView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    getTilesInView();
}

void MapView::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta()/8;
    int numSteps = numDegrees/15;


    if (event->orientation() == Qt::Vertical && isZoomable())
    {
        QPoint centre = QPoint(width()/2,height()/2);
        QPoint mouse = event->pos();

        //centerOn(mapToScene(event->pos()));
        centerOn(mapToScene(mouse + (centre - mouse)*pow(2.0,-numSteps)));

        zoom(numSteps);
    }
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    getTilesInView();
    removeDistantTiles(4);
}



void MapView::getTilesInView()
{
    if (MapSource.id == None) return;
    //TODO: extend this function so that the tiles are not requested from upper left to lower right
    // corner, but from the center outward.
    QRectF viewRect = mapToScene(0,0,width(),height()).boundingRect();
    QList<QPoint> XYTileList = getXYTileInRange(currentZoom,
                                                getLongFromMercatorX(viewRect.x()+viewRect.width()),
                                                getLongFromMercatorX(viewRect.x()),
                                                getLatFromMercatorY(viewRect.y()),
                                                getLatFromMercatorY(viewRect.y()+viewRect.height()) );
    QPoint p1 = XYTileList.first();
    QPoint p2 = XYTileList.last();
    QPointF center = QPointF( (p2.x()+p1.x()) / 2. , (p1.y()+p2.y()) / 2. );

    //sorts list so that points closest to center are loaded first
    QMap<qreal, QPoint> map;
    for(int i=0;i<XYTileList.size();++i) map.insertMulti(lengthSquared((XYTileList.at(i))-center),XYTileList.at(i));

    XYTileList = map.values();

    QPoint p;
    while(XYTileList.size()>0)
    {
        if (currentRequests<requestLimit) p = XYTileList.takeFirst();
        else p = XYTileList.takeLast();

        if (!tilePlaced(p,currentZoom)) getTile(p.x(),p.y(),currentZoom);
    }

    qDebug("Number of items in scene: "+QString::number(scene()->items().count()));
}

bool MapView::tileRequested(int x, int y, int z)
{
    if (allRequests.contains(QVector3D(x,y,z))) return true; else return false;
}

bool MapView::tilePlaced(const QPoint & coords, int z)
{
    for(int i = tiles[z].count()-1; i>=0; --i)
    {
        if (tiles[z].at(i)->coords() == coords) return true;
    }
    return false;
}

void MapView::getTile(int x, int y, int z)
{
    if (MapSource.id == None) return;

    //TODO: check if present in DB
    QByteArray tile = getTileFromDB(z,x,y,MapSource.url);
    if (QString::fromAscii(tile)!="0"){ //checks if tile is actually in DB
        placeTile(tile, x, y, z);
    } else if (currentZoom == z && !tileRequested(x,y,z)) //why currentZoom == z?
    {   QNetworkRequest request(QUrl(qPrintable(
                                         MapSource.url+QString::number(z)+"/"+
                                         QString::number(x)+"/"+
                                         QString::number(y)+".png")));


        if (currentRequests<requestLimit)
        {
            networkManager->get(request);
            ++currentRequests;
        } else {
            requestStack.push(request);
            if (requestStack.count() > 30) {
                int* x = getXYZFromUrl(requestStack.at(0).url().toString());
                allRequests.removeAll(QVector3D(x[0],x[1],x[2]));

                delete[] x;

                requestStack.remove(0);
            }
        }
        allRequests.append(QVector3D(x,y,z));
    }
}

int* MapView::getXYZFromUrl(const QString & url)
{
    int *ret = new int[3];

    //Format/Meanings: http://domain.tld/path/filename.end
    //The following assumes that the path has the following form */zoomLevel/tileX/tileY.end
    ret[2] = url.section('/',-3,-3).toInt(); //z
    ret[0] = url.section('/',-2,-2).toInt(); //x
    ret[1] = url.section('/',-1,-1).remove(-4,4).toInt(); //y
    return ret;
}

void MapView::placeTile(QByteArray tileData, int x, int y, int z)
{
        QPixmap tilePixmap;
        tilePixmap.loadFromData(tileData);
        OSMTile *tile = new OSMTile(tilePixmap, QPoint(x,y), z);

        placeTile(tile);
}

void MapView::placeTile(OSMTile *tile)
{
    scene()->addItem(tile);
    tiles[(int)(tile->zValue())].append(tile);
}

void MapView::removeTile(OSMTile *tile)
{
    scene()->removeItem(tile);
    tiles[(int)(tile->zValue())].removeAll(tile);
    delete tile;
}

void MapView::removeTiles(int z)
{
    QList<OSMTile *> tilesAtZ = tiles[z];
    for (int i = tilesAtZ.count()-1; i>=0; --i)
    {
        removeTile(tilesAtZ.at(i));
    }
}

void MapView::updateOutlines()
{
    outlinePen.setWidthF(pow(2.0, -currentZoom-2));
    for (int i = 0; i < routes.count(); ++i)
    {
        routes.at(i)->pathOutline()->setPen(outlinePen);
    }
}

void MapView::removeDistantTiles(qreal factor)
{
    //Removes items that are not partially in the extended viewRect
    QRectF viewRect = mapToScene(0,0,width(),height()).boundingRect();
    QRectF extendedViewRect = viewRect; extendedViewRect.setSize(extendedViewRect.size()*factor);
                                        extendedViewRect.moveCenter(viewRect.center());
    QList<QGraphicsItem *> allItems = scene()->items();
    QList<QGraphicsItem *> visibleItems = scene()->items(extendedViewRect,Qt::IntersectsItemBoundingRect);
    for (int i = allItems.count()-1; i>=0; --i)
    {
        //removes tiles not in viewrect
        if (!visibleItems.contains(allItems.at(i)) && allItems.at(i)->zValue()<18) removeTile(dynamic_cast<OSMTile*>(allItems.at(i)));
    }

}

void MapView::zoom(int dZoom)
{
    int newZoom = currentZoom + dZoom;
    if (newZoom > MapSource.maxzoom || newZoom < MapSource.minzoom || dZoom == 0)
        {getTilesInView(); return;}

    scale(pow(2,dZoom),pow(2,dZoom));
    currentZoom += dZoom;
    emit zoomChanged(currentZoom);

    //remove tiles that are not in the current zoomlevel or up to maxLayers-1 above
    int maxLayers = 4;
    for(int i = 0; i<=MapSource.maxzoom; ++i)
    {
        if (i < currentZoom - (maxLayers - 1)|| i > currentZoom) removeTiles(i);
    }

    updateOutlines();
    removeDistantTiles();
    getTilesInView();
}

void MapView::zoomTo(int level)
{
    zoom(level-currentZoom);
}

void MapView::centerMapOnPath(QGraphicsPathItem *path)
{
    if (path == 0) {qDebug("cMoP: path is null!"); return; }

    QRect view = QRect(0,0,width(),height());
    QPoint center = view.center();
    QRect nextView = view;
    nextView.setSize(view.size()/2);
    nextView.moveCenter(center);

    QRectF pathBoundingRect = path->boundingRect();

    //if the item has no path data, abort
    if(pathBoundingRect.width() == 0.0)
    {
        qDebug("This item has no valid path");
        return;
    }

    centerOn(path);
    if (mapToScene(view).boundingRect().contains(pathBoundingRect))
    {
        centerOn(path);
        while (mapToScene(nextView).boundingRect().contains(pathBoundingRect)) {zoom(1);}
    } else {
        while (!mapToScene(view).boundingRect().contains(pathBoundingRect)) {zoom(-1);}
    }
}

void MapView::centerMapOnRoute(ActiveRouteListItem *item)
{
    centerMapOnPath(item->path());
}

void MapView::add(ActiveRouteListItem *route)
{
    if (route == 0) return;

    QGraphicsPathItem* path = route->path();


    if (path != 0)
    {
        //create route outline
        //if (item->pathOutline() != 0) scene()->addItem(item->pathOutline());
        QGraphicsPathItem* outline = new QGraphicsPathItem(path->path(), path);
        route->setPathOutline(outline);
        outline->setZValue(19);
        outline->setOpacity(0.7);
        outline->setPen(outlinePen);

        updateOutlines();
        scene()->addItem(outline);

        scene()->addItem(path);
        path->setZValue(20);
        routes.append(route);
    }

}

void MapView::remove(ActiveRouteListItem *route)
{
    routes.removeAll(route);
    delete route->path();
    delete route->pathOutline();
}

void MapView::setMapSource(sourceName source)
{
    osmMapSource newSource = {None,"None","",0,20};
    QMap<sourceName,osmMapSource>::const_iterator it = mapSources.find(source);

    //check if new map source is valid
    if (it != mapSources.end() && it.key() == source) newSource = it.value();

    //check if map source changed, if it did not, abort
    if (newSource.id == MapSource.id) return;
    emit mapSourceChanged(newSource.minzoom, newSource.maxzoom);

    //remove all tiles, resize tilemanager
    if (MapSource.id != None) {
    for (int z = MapSource.minzoom; z <= MapSource.maxzoom; ++z) removeTiles(z);
    delete[] tiles;
    }
    if (newSource.id != None) tiles = new QList<OSMTile*>[newSource.maxzoom+1];

    MapSource = newSource;

    if (currentZoom > MapSource.maxzoom) zoomTo(MapSource.maxzoom);
    else if (currentZoom < MapSource.minzoom) zoomTo(MapSource.minzoom);
    else getTilesInView();
}

int MapView::zoomLevel() const
{
    return currentZoom;
}

bool MapView::isZoomable() const
{
    return Zoomable;
}

void MapView::setZoomable(bool zoomable)
{
    Zoomable = zoomable;
}
