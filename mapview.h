/*
 * teaview.h
 *
 *  Created on: 02.01.2010
 *      Author: jf
 */

#ifndef MAPVIEW_H_
#define MAPVIEW_H_

#include <QObject>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>

#include <QMouseEvent>
#include <QWheelEvent>

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
//#include <QtNetwork>

#include <QStack>
#include <QList>
//#include <QNetwork

#include <db.h>

#include <activeroutelistitem.h>

#include <math.h>
#include <coordinates.h>


class OSMTile : public QGraphicsPixmapItem
{
public:
    explicit OSMTile(QPixmap & pixmap, const QPoint & coords, int z);
    QPoint coords() const;

private:
    const QPoint Coordinates;
};

//todo namespace
enum sourceName{None, Mapnik, OCM};

struct osmMapSource {
    sourceName id;
    QString name;
    QString url;
    int minzoom;
    int maxzoom;
};

//TAT: private QGraphicsView
class MapView : public QGraphicsView {
	Q_OBJECT

public:
        MapView(QWidget *parent = 0);
        virtual ~MapView();

        void addPath(QGraphicsPathItem* path);
        void add(ActiveRouteListItem* route);
        void remove(ActiveRouteListItem* route);

        void centerMapOnPath(QGraphicsPathItem* path);
        void centerMapOnRoute(ActiveRouteListItem* item);

        void setMapSource(sourceName source);
        osmMapSource mapSource();

        bool isZoomable() const;
        void setZoomable(bool zoomable);

        void zoom(int steps);
        void zoomTo(int level);
        int zoomLevel() const;

signals:
        void zoomChanged(int);
        void mapSourceChanged(int, int);
        void noNetwork();

public slots:

private slots:
        void tileDownloaded(QNetworkReply* reply);

protected:
        virtual void wheelEvent(QWheelEvent *event);
        virtual void mouseReleaseEvent(QMouseEvent *event);
        virtual void resizeEvent(QResizeEvent *event);

private:
        void connectSignalsAndSlots();

        void getTilesInView();
        void getTile(int x, int y, int z);
        void requestTile(int x, int y, int z);
        void placeTile(QByteArray tile, int x, int y, int z); //todo byte array pointer
        void placeTile(OSMTile* tile);

        bool tilePlaced(const QPoint & coords, int z);
        bool tileRequested(int x, int y, int z);

        int* getXYZFromUrl(const QString & url);

        void removeTile(OSMTile* tile);
        void removeTiles(int z);
        void removeDistantTiles(qreal factor = 2);


        void updateOutlines();

        bool Zoomable;

        QList<OSMTile*>* tiles;
        QList<ActiveRouteListItem*> routes;


        QNetworkAccessManager* networkManager;
        QStack<QNetworkRequest> requestStack;
        int currentRequests;
        QList<QVector3D> allRequests;

        int requestLimit;

        QPen outlinePen;

        osmMapSource MapSource;
        QMap<sourceName,osmMapSource> mapSources;

        int currentZoom;
};

#endif /* MAPVIEW_H_ */
