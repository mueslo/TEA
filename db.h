/*
 * db.h
 *
 *  Created on: 18.05.2009
 *      Author: jf
 */

#ifndef DB_H_
#define DB_H_

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdlib.h>
#include "hashlibpp.h"
#include <QtSql>
#include <coordinates.h>
#include <QMessageBox>

//TODO: main DB
//TODO: map DB
//TODO: delete DB elements
//TODO: check if file is valid before import


/*inline QSqlRecord dbqry_out(QString query)
{
	QSqlQuery dbquery;
	dbquery.exec(qPrintable(query));
	dbquery.first();
	return dbquery.record();
}

inline void dbqry_in(QString query)
{
	QSqlQuery dbquery;
	dbquery.exec(qPrintable(query));
}
*/

QSqlQuery customRDBQuery(QString query);
bool prepareActiveRoutesDB();
bool prepareRoutesDB();
bool prepareMapsDB();
bool initialiseDBs();
void closeDBs();
QString generateChecksumFromFile(QString FilePath);
QSqlQuery getCurrentlyLoadedRoutes();
bool routePresentInADB(QString checksum);
bool rrouteLoadedInADB(QString uid);
bool routePresentInRDB(QString checksum);
bool routePresentInDBs(QString checksum);
QSqlQuery getAllMetadata(QString database);
void generateMetadataFromRoute(QString auid);
QString importRoute(QString TEAFilePath);
QSqlRecord getRouteMetadata(QString uid, QString database);
QSqlQuery getRouteData(QString uid, QString database);
void loadRoute(QString uid);
//Returns now the UID under which the route was stored in rdb
QString saveRoute(QString auid);
void deleteRoute(QString uid, QString database);
void setMetadata(QString auid, QString data, QString type);
void setRouteName(QString auid, QString name);
void setRouteTags(QString auid, QString tags);
void setRouteID(QString auid, QString id);
void setRouteLocation(QString auid, QString location);
void setRoutePicture(QString auid, QByteArray picture);
void addTileToDB(int zoomLevel, int tileX, int tileY, QByteArray tile, QString mapsource);
QByteArray getTileFromDB(int zoomLevel, int tileX, int tileY, QString mapsource);
#endif /* DB_H_ */
