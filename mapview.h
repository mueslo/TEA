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

//todo namespace
enum sourceName{None, Mapnik, OCM};

struct osmMapSource {
    sourceName id;
    QString name;
    QString url;
    int minzoom;
    int maxzoom;
};

class OSMTile : public QGraphicsPixmapItem
{
public:
    explicit OSMTile(QPixmap & pixmap, const QPoint & coords, int z);
    QPoint coords() const;

private:
    const QPoint Coordinates;
};

class TileRequest : public QNetworkRequest
{
public:
    explicit TileRequest(const int &x, const int &y, const int &z, const osmMapSource & source);
    explicit TileRequest(const int &x, const int &y, const int &z, sourceName source);
    explicit TileRequest(const int &x, const int &y, const int &z, const QString & sourceURL);
    explicit TileRequest(const QNetworkReply *reply);

    bool operator==(const TileRequest & request) const;

    int x() const;
    int y() const;
    int z() const;
    osmMapSource mapSource() const;


private:
    int X;
    int Y;
    int Z;
    osmMapSource MapSource;

};

//TAT: private QGraphicsView, probably nonsense
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
        osmMapSource mapSource(sourceName source);

        bool isInteractable() const;
        void setInteractable(bool interactive);

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
        virtual void mousePressEvent(QMouseEvent *event);
        virtual void mouseMoveEvent(QMouseEvent *event);
        virtual void mouseReleaseEvent(QMouseEvent *event);
        virtual void resizeEvent(QResizeEvent *event);
        virtual void keyPressEvent(QKeyEvent *event);

private:
        void connectSignalsAndSlots();

        void getTilesInView();
        void getTile(int x, int y, int z);
        void requestTile(int x, int y, int z);
        void placeTile(QByteArray* tile, int x, int y, int z); //todo byte array pointer
        void placeTile(OSMTile* tile);

        bool tilePlaced(const QPoint & coords, int z);

        void removeTile(OSMTile* tile);
        void removeTiles(int z);
        void removeDistantTiles(qreal factor = 2);


        void updateOutlines();

        bool Interactive;

        QList<OSMTile*>* tiles;
        QList<ActiveRouteListItem*> routes;

        QNetworkAccessManager* networkManager;
        QList<TileRequest> requestStack;
        QList<TileRequest> currentRequests;

        int requestLimit;
        int maxReqStackSize;

        QPen outlinePen;

        QTime lastTileUpdate;
        QTime *currentTime;

        osmMapSource MapSource;
        QMap<sourceName,osmMapSource> mapSources;

        int currentZoom;
};

#endif /* MAPVIEW_H_ */
