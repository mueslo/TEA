/*
 * coordinates.h
 *
 *  Created on: 04.10.2009
 *      Author: jf
 */

#ifndef COORDINATES_H_
#define COORDINATES_H_
#define PI 3.1415926535897932384626433832795

#include <math.h>
#include <iostream>
#include <QPointF>
#include <QList>

using namespace std;

inline double getLonFromRawLon(QString lon)
{
	double nlon = lon.left(4).right(3).toDouble()+lon.right(6).toDouble()/600000;
	if (lon.left(1)=="1") return nlon;
	else if (lon.left(1)=="0") return -nlon;
	return 0;
}

inline double getLatFromRawLat(QString lat)
{
	double nlat = lat.left(3).right(2).toDouble()+lat.right(6).toDouble()/600000;
	if (lat.left(1)=="1") return nlat;
	else if (lat.left(1)=="0") return -nlat;
	return 0;
}

inline int getXFromLon(QString lon)
{
	//M: arc1minutes
	//D: degrees
	// input of type DDMM(.)MMMM
	//output of type DD(.)DDDDDD
	int xnumeric = lon.left(4).right(3).toInt()*1000000+5*lon.right(6).toInt()/3;
		//					DDD------                  +1000000* (MMMMMM/10000)/60
		//					DDD------				   +1000000*     MM.MMMM    /60
		//					DDD------				   +1000000*    (.)DDDDDD
	if (lon.left(1)=="1") return xnumeric;
	else if (lon.left(1)=="0") return -xnumeric;
	return 0;
}

inline int getYFromLat(QString lat)
{
	int ynumeric = lat.left(3).right(2).toInt()*1000000+5*lat.right(6).toInt()/3;
	if (lat.left(1)=="1") return -ynumeric;
	else if (lat.left(1)=="0") return ynumeric;
	return 0;
}

//see http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames for the 2 below

inline double getMercatorYFromLat(double latitude)
{
	return -atanh(sin(   latitude*PI/180  ));
}

inline double getMercatorXFromLon(double longitude)
{
	return longitude*PI/180;
}

inline double getLatFromMercatorY(double y)
{
	return -atan(sinh( y ))*180/PI;
}

inline double getLongFromMercatorX(double x)
{
	return x*180/PI;
}

inline double getDistanceFromRawCoords(QString latitude1Raw, QString longitude1Raw, QString latitude2Raw, QString longitude2Raw)
{

	double latitude1 = 2*PI*(getYFromLat(latitude1Raw)/1000000)/360;
	double longitude1 = 2*PI*(getXFromLon(longitude1Raw)/1000000)/360;
	double latitude2 = 2*PI*(getYFromLat(latitude2Raw)/1000000)/360;
	double longitude2 = 2*PI*(getXFromLon(longitude2Raw)/1000000)/360;
	//spherical law of cosines:
	//double distance = acos(sin(latitude1)*sin(latitude2)+cos(latitude1)*cos(latitude2)*cos(longitude2-longitude1))*40024/(2*PI);

	//haversine formula: (more accurate at small distances)
	double dLat = (latitude2-latitude1);
	double dLon = (longitude2-longitude1);
	double a = sin(dLat/2)*sin(dLat/2)+cos(latitude1)*cos(latitude2)*sin(dLon/2)*sin(dLon/2);
	double distance = 40024/(PI) * atan2(sqrt(a), sqrt(1-a));
	return distance;
}

inline int longitudeToXTile(double longitude, int zoomLevel)
{
	return (int)(floor((longitude + 180.0) / 360.0 * pow(2.0, zoomLevel)));
}

inline int latitudeToYTile(double latitude, int zoomLevel)
{
	return (int)(floor((1.0 - log( tan(latitude * PI/180.0) + 1.0 / cos(latitude * PI/180.0)) / PI) / 2.0 * pow(2.0, zoomLevel)));
}

inline double XTileToLongitude(int xtile, int zoomLevel)
{
	return (-180+360*pow(-2,zoomLevel)*xtile);
}

inline double YTileToLatitude(int ytile, int zoomLevel)
{
	double n = PI - 2.0 * PI * ytile / pow(2.0, zoomLevel);
	return 180.0 / PI * atan(0.5 * (exp(n) - exp(-n)));
}

inline QPointF tileToCoord(int xtile, int ytile, int zoomLevel)
{
	return QPointF(-PI + 2.0 * PI * xtile / pow(2.0, zoomLevel),-PI + 2.0 * PI * ytile / pow(2.0, zoomLevel));
}

inline QList<QPoint> getXYTileInRange(int zoomLevel, double max_longitude, double min_longitude, double max_latitude, double min_latitude)
{
	int i,j;
	QList<QPoint> XYList;

        //cout << "minlon " << min_longitude << endl;
        //cout << "maxlon " << max_longitude << endl;
        //cout << "minlat " << min_latitude << endl;
        //cout << "maxlat " << max_latitude << endl;

	if (max_latitude < -85.0511  || min_latitude > 85.0511) {max_latitude = 0; min_latitude = 0;}
	if (max_latitude >= 85.0511) max_latitude = 85.0511;
	if (min_latitude <= -85.0511) min_latitude = -85.0511;
	if (max_longitude < -180  || min_longitude > 180) {max_longitude = 0; min_longitude = 0;}
	if (max_longitude >= 180) max_longitude = 179.999999999999;
	if (min_longitude <= -180) min_longitude = -179.999999999999;

        //cout << "minlon " << min_longitude << endl;
        //cout << "maxlon " << max_longitude << endl;
        //cout << "minlat " << min_latitude << endl;
        //cout << "maxlat " << max_latitude << endl;

        int max_ytile = (latitudeToYTile(min_latitude, zoomLevel));
        int min_ytile = (latitudeToYTile(max_latitude, zoomLevel));
	int max_xtile = (longitudeToXTile(max_longitude, zoomLevel));
	int min_xtile = (longitudeToXTile(min_longitude, zoomLevel));

        //cout << "max_ytile=" << max_ytile << endl;
        //cout << "min_ytile=" << min_ytile << endl;
        //cout << "max_xtile=" << max_xtile << endl;
        //cout << "min_xtile=" << min_xtile << endl;

        //cout << "Zoom level: " << zoomLevel << endl;
	for (i=min_xtile;i<=max_xtile;i++)
	{
		for (j=min_ytile;j<=max_ytile;j++)
		{
			XYList << QPoint(i,j);
                        //cout << "Tile: x=" << i << ", y=" << j << endl;
		}
	}
	return XYList;
}
/*
bool ShorterThan(const QPointF & a, const QPointF & b)
{
    return a.x()*a.x() + a.y()*a.y() < b.x()*b.x() + b.y()*b.y();
}*/

inline qreal lengthSquared(const QPointF & a)
{
    return a.x()*a.x()+a.y()*a.y();
}

#endif /* COORDINATES_H_ */
