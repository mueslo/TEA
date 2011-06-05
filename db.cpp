#include "db.h"
//TODO: main DB
//TODO: map DB
//TODO: delete DB elements
//TODO: check if file is valid before import

using namespace std;

/*QSqlRecord dbqry_out(QString query)
{
	QSqlQuery dbquery;
	dbquery.exec(qPrintable(query));
	dbquery.first();
	return dbquery.record();
}

void dbqry_in(QString query)
{
	QSqlQuery dbquery;
	dbquery.exec(qPrintable(query));
}
*/

QSqlQuery customRDBQuery(QString query)
{
    QSqlQuery rdbquery(QSqlDatabase::database("rdb"));
    rdbquery.exec(qPrintable(query));
    return rdbquery;
}

bool prepareActiveRoutesDB()
{
	if (QSqlDatabase::database("adb").open())
	{
		QSqlQuery adbquery(QSqlDatabase::database("adb"));
		adbquery.exec(	"CREATE TABLE active_metadata (" //0
						"auid integer primary key," //1
						"uid integer," //2
						"date text," //3
						"id integer," //4
						"location text," //5
						"tags text," //6
						"name text," //7
						"checksum text," //8
						"distance text," //9 in kilometres
						"altitude_gain integer," //10 in decimetres
						"altitude_loss integer, " //11 in decimetres
						"total_altitude_difference integer, " //12 in decimetres
						"duration text," //13 in format hhmmss
						"maximum_latitude integer," //14 in format ddmmmmmm
						"minimum_latitude integer," // 15 -"-
						"maximum_longitude integer," //16 in format dddmmmmmm
						"minimum_longitude integer," //17 -"-
						"average_elevation integer," //18 in decimetres
						"average_velocity integer," //19 in kilometres per hour
						"picture blob," //20
						"num_nodes integer," //21
						"standard text," //22
						"time_between_nodes integer)"); //23


		adbquery.exec(	"CREATE TABLE route("
						"node integer primary key,"
						"velocity integer,"
						"frequency integer,"
						"slope integer,"
						"latitude integer,"
						"longitude integer,"
						"altitude integer,"
						"time text,"
						"date text)"); //flag erstmal nicht
		adbquery.exec(	"CREATE TABLE settings("
						"next_auid integer,"
						"empty integer,"
						"changes integer)");
		//adbquery.exec( "SELECT * FROM settings" );
		/*if(adbquery.first() == 0)*/ adbquery.exec("INSERT INTO settings(next_auid) VALUES (0)");
					      adbquery.finish();
		qDebug("adb success");
			return true;
	} else return false;
}

bool prepareRoutesDB()
{
	if (QSqlDatabase::database("rdb").open())
	{
		QSqlQuery rdbquery(QSqlDatabase::database("rdb"));
		rdbquery.exec(	"CREATE TABLE IF NOT EXISTS metadata ("
						"uid integer primary key,"
						"date text,"
						"id integer,"
						"location text,"
						"tags text,"
						"name text,"
						"checksum text,"
						"distance text,"
						"altitude_gain integer,"
						"altitude_loss integer, "
						"total_altitude_difference integer, "
						"duration text,"
						"maximum_latitude integer,"
						"minimum_latitude integer,"
						"maximum_longitude integer,"
						"minimum_longitude integer,"
						"average_elevation integer,"
						"average_velocity integer,"
						"picture blob,"
						"num_nodes integer,"
						"standard text,"
						"time_between_nodes integer)");
		rdbquery.exec(	"CREATE TABLE IF NOT EXISTS rroute("
						"node integer primary key,"
						"velocity integer,"
						"frequency integer,"
						"slope integer,"
						"latitude integer,"
						"longitude integer,"
						"altitude integer,"
						"time text,"
						"date text)"); //flag erstmal nicht
		rdbquery.exec(	"CREATE TABLE IF NOT EXISTS rsettings("
						"next_uid integer,"
						"empty integer,"
						"changes integer)");
		rdbquery.exec( "SELECT * FROM rsettings" );
		if(rdbquery.first() == 0) rdbquery.exec("INSERT INTO rsettings(next_uid) VALUES (0)");
		qDebug("rdb success");
			return true;
	} else return false;
}


bool prepareMapsDB()
{
	if (QSqlDatabase::database("mdb").open())
		{
			QSqlQuery mdbquery(QSqlDatabase::database("mdb"));
			for (int i=0;i<=18;i++)
			{
			mdbquery.exec(	"CREATE TABLE IF NOT EXISTS zoom"+QString::number(i)+" ("
							"xtile integer,"
							"ytile integer,"
							"tile string,"
							"mapsource string)");
			}
			qDebug("mdb success");
				return true;
		} else return false;
}


bool initialiseDBs()
//returns false if QSqlDatabase::open() failed
{
	QSqlDatabase adb = QSqlDatabase::addDatabase("QSQLITE","adb");
	QSqlDatabase rdb = QSqlDatabase::addDatabase("QSQLITE","rdb");
	QSqlDatabase mdb = QSqlDatabase::addDatabase("QSQLITE","mdb");

	adb.setDatabaseName(":memory:");
	//adb.setDatabaseName("a.db");
	rdb.setDatabaseName("routes.db");
	mdb.setDatabaseName("maps.db");

	if (!adb.isValid()) cout << "Error: adb is not valid" << endl;
	if (!rdb.isValid()) cout << "Error: rdb is not valid" << endl;
	if (!mdb.isValid()) cout << "Error: mdb is not valid" << endl;

	//adb.setDatabaseName("C:/Users/jf/Desktop/test.sqlite");
	if (prepareActiveRoutesDB() && prepareRoutesDB() && prepareMapsDB()) return true; else return false;
}





void closeDBs()
{
	QSqlDatabase::database("adb").close();
	QSqlDatabase::database("rdb").close();
	QSqlDatabase::database("mdb").close();

	QSqlDatabase::removeDatabase("adb");
	QSqlDatabase::removeDatabase("rdb");
	QSqlDatabase::removeDatabase("mdb");

}

QString generateChecksumFromFile(QString FilePath)
{
	hashwrapper *md5 = new md5wrapper();
	QString sMD5;
	try
	{
		sMD5 = sMD5.fromStdString(md5->getHashFromFile(FilePath.toStdString()));
		return sMD5;
	}
			catch (hlException &e)
			{
				std::cerr << "Error("
					  << e.error_number()
					  << "): "
					  << e.erro_message()
					  << std::endl;
				exit(e.error_number());
			}
			delete md5;
}

QSqlQuery getCurrentlyLoadedRoutes()
{
	QSqlQuery adbquery(QSqlDatabase::database("adb"));
	adbquery.exec("SELECT auid FROM active_metadata");
	if (!adbquery.isActive()) cout << "aDB error (db)" << endl;
	return adbquery;
}

bool routePresentInADB(QString checksum)
{
	QSqlQuery adbquery(QSqlDatabase::database("adb"));
	adbquery.prepare("SELECT * FROM active_metadata WHERE checksum = :checksum");
	adbquery.bindValue(":checksum", checksum);
	adbquery.exec();
	if (!adbquery.isActive()) cout << "aDB error (importing)" << endl;
	if (adbquery.next()) return true; else return false;
}

bool rrouteLoadedInADB(QString uid)
{
	QSqlQuery adbquery(QSqlDatabase::database("adb"));
	adbquery.prepare("SELECT * FROM active_metadata WHERE uid = :uid");
	adbquery.bindValue(":uid", uid);
	adbquery.exec();
	if (!adbquery.isActive()) cout << "aDB error (loading)" << endl;
	if (adbquery.next()) return true; else return false;
}

bool routePresentInRDB(QString checksum)
{
	QSqlQuery rdbquery(QSqlDatabase::database("rdb"));
	rdbquery.prepare("SELECT * FROM metadata WHERE checksum = :checksum");
	rdbquery.bindValue(":checksum", checksum);
	rdbquery.exec();
	if (!rdbquery.isActive()) cout << "rDB error (importing)" << endl;
	if (rdbquery.next()) return true; else return false;
}

bool routePresentInDBs(QString checksum)
{
	if (routePresentInADB(checksum) || routePresentInRDB(checksum)) return true; else return false;
}

QSqlQuery getAllMetadata(QString database)
{
	QSqlQuery dbquery(QSqlDatabase::database(database));
	if (database=="adb")
	    dbquery.exec("SELECT * FROM active_metadata");
	else if (database=="rdb") dbquery.exec("SELECT * FROM metadata");
	return dbquery;
}

void generateMetadataFromRoute(QString auid)
{
	QSqlQuery adbquery(QSqlDatabase::database("adb"));
	QString data,data1;
	int temp1, temp2, temp3;
	double temp4;

	//Number of nodes
	adbquery.exec(qPrintable("SELECT max(node) FROM route"+auid));
	adbquery.first();
	temp1 = adbquery.record().value(0).toInt();
	adbquery.exec(qPrintable("UPDATE active_metadata "
							"SET num_nodes="+QString::number(temp1)+
							" WHERE auid="+auid));

	//Date
	adbquery.exec(qPrintable("SELECT min(date) FROM route"+auid));
	adbquery.first();
	data = adbquery.record().value(0).toString();
	adbquery.exec(qPrintable("UPDATE active_metadata "
							"SET date='"+data+
							"' WHERE auid="+auid));

	//Maximum latitude
	adbquery.exec(qPrintable("SELECT max(latitude) FROM route"+auid));
	adbquery.first();
	data = adbquery.record().value(0).toString();
	adbquery.exec(qPrintable("UPDATE active_metadata "
							"SET maximum_latitude="+QString::number(getYFromLat(data))+
							" WHERE auid="+auid));

	//Minimum latitude
	adbquery.exec(qPrintable("SELECT min(latitude) FROM route"+auid));
	adbquery.first();
	data = adbquery.record().value(0).toString();
	adbquery.exec(qPrintable("UPDATE active_metadata "
							"SET minimum_latitude="+QString::number(getYFromLat(data))+
							" WHERE auid="+auid));

	//Maximum longitude
	adbquery.exec(qPrintable("SELECT max(longitude) FROM route"+auid));
	adbquery.first();
	data = adbquery.record().value(0).toString();
	adbquery.exec(qPrintable("UPDATE active_metadata "
							"SET maximum_longitude="+QString::number(getXFromLon(data))+
							" WHERE auid="+auid));

	//Minimum longitude
	adbquery.exec(qPrintable("SELECT min(longitude) FROM route"+auid));
	adbquery.first();
	data = adbquery.record().value(0).toString();
	adbquery.exec(qPrintable("UPDATE active_metadata "
							"SET minimum_longitude="+QString::number(getXFromLon(data))+
							" WHERE auid="+auid));

	//Duration
	adbquery.exec(qPrintable("SELECT time FROM route"+auid));
	adbquery.first();
	data=adbquery.record().value(0).toString(); //QND
	if(data.length() != 6) data.prepend("0");
	QTime time1=QTime::fromString(data,"hmmss");
	qDebug(time1.toString());

	adbquery.last();
	data=adbquery.record().value(0).toString(); //QND
	if(data.length() != 6) data.prepend("0");
	QTime time2=QTime::fromString(data,"hmmss");
	qDebug(time2.toString());
	adbquery.exec(qPrintable("UPDATE active_metadata "
							"SET duration='"+(QTime::fromString("000","hms").addSecs(time1.secsTo(time2))).toString("HHmmss")+
							"' WHERE auid="+auid));

	//Total altitude difference
	adbquery.exec(qPrintable("SELECT altitude FROM route"+auid));
	adbquery.first(); temp1=adbquery.record().value(0).toInt();
	adbquery.last(); temp2=adbquery.record().value(0).toInt();
	adbquery.exec(qPrintable("UPDATE active_metadata "
							"SET total_altitude_difference="+QString::number(temp2-temp1)+
							" WHERE auid="+auid));

	//Height gain+loss, distance
	temp1 = 0; temp2 = 0; temp4 = 0;
	adbquery.exec(qPrintable("SELECT altitude, latitude, longitude FROM route"+auid));
	adbquery.first();
	temp3=adbquery.record().value(0).toInt();
	data = adbquery.record().value(1).toString();
	data1 = adbquery.record().value(2).toString();
	while (adbquery.next())
	{
		temp4 += getDistanceFromRawCoords(
				data,
				data1,
				adbquery.record().value(1).toString(),
				adbquery.record().value(2).toString());


		if (temp3 < adbquery.record().value(0).toInt())
		    temp1 += (adbquery.record().value(0).toInt()-temp3);
			else temp2 += (adbquery.record().value(0).toInt()-temp3);

		temp3 = adbquery.record().value(0).toInt();
		data = adbquery.record().value(1).toString();
		data1 = adbquery.record().value(2).toString();

	}
	adbquery.exec(qPrintable("UPDATE active_metadata "
							"SET altitude_gain="+QString::number(temp1)+", "
								"altitude_loss="+QString::number(temp2)+", "
								"distance="+QString::number(temp4)+
							" WHERE auid="+auid));

	//Average elevation
	adbquery.exec(qPrintable("SELECT avg(altitude) FROM route"+auid));
	adbquery.first();
	data=adbquery.record().value(0).toString();
	adbquery.exec(qPrintable("UPDATE active_metadata "
							"SET average_elevation="+data+
							" WHERE auid="+auid));


	//Average velocity

	/*adbquery.exec(qPrintable("SELECT duration, distance FROM active_metadata WHERE auid="+auid));
	adbquery.first();
	data=adbquery.record().value(0).toString(); data1=adbquery.record().value(1).toString();
	double timeInSecs=QTime::fromString("000000","HHmmss").secsTo(QTime::fromString(data,"HHmmss"));
	temp4=data1.toDouble() / (timeInSecs/3600);
	adbquery.exec(qPrintable("UPDATE active_metadata "
							"SET average_velocity="+QString::number(temp4)+
							" WHERE auid="+auid));*/

	adbquery.exec(qPrintable("SELECT avg(velocity) FROM route"+auid));
	adbquery.first();
	temp4=adbquery.record().value(0).toDouble() * 0.01;
	data=QString::number(temp4, 'g', 3);
	//data=adbquery.record().value(0).toString();
	adbquery.exec(qPrintable("UPDATE active_metadata "
				 "Set average_velocity="+data+
				 " WHERE auid="+auid));
}

QString importRoute(QString TEAFilePath)
{
	QSqlQuery adbquery(QSqlDatabase::database("adb"));
	fstream TEAFile;
	TEAFile.open(TEAFilePath.toStdString().c_str(),ios::in);
	string line;
	unsigned int characterNum;
	QString auid,stringBuffer;
	adbquery.exec("SELECT max(next_auid) from settings");
	adbquery.first(); cout << "Next AUID: " <<qPrintable(adbquery.record().value(0).toString()) << endl;
	auid=adbquery.record().value(0).toString();
	adbquery.exec(qPrintable("CREATE TABLE route"+auid+" AS SELECT * FROM route"));
	adbquery.exec(qPrintable("UPDATE settings SET next_auid="+QString::number((auid.toInt()+1))+
							" WHERE next_auid="+auid));

	//get header
	//compliant up to TEA standard 0.1
	getline(TEAFile,line);
	QString firstLine = QString::fromStdString(line);
	bool header = false;
    if (firstLine.startsWith("$TEA"))
    {
	header = true;
	qDebug("Header: "+firstLine);
	firstLine.remove(0,4);
	firstLine.replace(";",",");


	} else {
	    qDebug("No header");
	    //characterNum=0;

	    for((characterNum=0);(characterNum<(line.size()));characterNum++)
	    {
		if (line[characterNum] != ';') stringBuffer+=line[characterNum];
		else stringBuffer += ", ";
	    }

	    adbquery.exec(qPrintable("INSERT INTO route"+auid+
				     " VALUES("+stringBuffer+")"));
	    stringBuffer.clear();

	}

	//get remaining raw data
	qDebug("Reading raw data");
	while (!TEAFile.eof())
	{
	    getline(TEAFile,line);
	    //characterNum=0;

	    for((characterNum=0);(characterNum<(line.size()));characterNum++)
	    {
		if (line[characterNum] != ';') stringBuffer+=line[characterNum];
		else stringBuffer += ", ";
	    }

	    adbquery.exec(qPrintable("INSERT INTO route"+auid+
				     " VALUES("+stringBuffer+")"));
	    stringBuffer.clear();
	}
	TEAFile.close();
	qDebug("Raw data read");
	if (header)
	adbquery.exec(qPrintable("INSERT INTO active_metadata(auid, checksum, standard, time_between_nodes) "
							"VALUES(" + auid + ", '" + generateChecksumFromFile(TEAFilePath) + "'" + firstLine +")"));
	else adbquery.exec(qPrintable("INSERT INTO active_metadata(auid, checksum) "
					 "VALUES(" + auid + ", '" + generateChecksumFromFile(TEAFilePath) + "')"));
	//qDebug(qPrintable("INSERT INTO active_metadata(auid, checksum, standard, time_between_nodes) "
	//		  "VALUES(" + auid + ", '" + generateChecksumFromFile(TEAFilePath) + "'" + firstLine +")"));

	generateMetadataFromRoute(auid);
	return auid;
}

QSqlRecord getRouteMetadata(QString uid, QString database)
{
	QSqlQuery dbquery(QSqlDatabase::database(database));
	if (database=="adb")
		dbquery.exec(qPrintable("SELECT * FROM active_metadata WHERE auid="+uid));
	else
		if (database=="rdb")
			dbquery.exec(qPrintable("SELECT * FROM metadata WHERE uid="+uid));

	dbquery.first();
	return dbquery.record();
}

QSqlQuery getRouteData(QString uid, QString database)
{
	QSqlQuery dbquery(QSqlDatabase::database(database));
	if (database == "adb") dbquery.exec(qPrintable("SELECT * FROM route"+uid));
	else if (database == "rdb") dbquery.exec(qPrintable("SELECT * FROM rroute"+uid));
	dbquery.first();
	return dbquery;
}

QString loadRoute(QString uid)
{
	qDebug("Loading route");
	QString next_auid;
	QSqlQuery adbquery(QSqlDatabase::database("adb"));
	QSqlQuery rdbquery(QSqlDatabase::database("rdb"));
	QSqlRecord metadata;

	//get metadata
	rdbquery.exec(qPrintable("SELECT * FROM metadata WHERE uid="+uid));
	rdbquery.first();
	metadata = rdbquery.record();

	if (!rdbquery.isActive())
		QMessageBox::warning(0, "aDatabase Error11",
				 adbquery.lastError().text());

	//get next_auid and update
	adbquery.exec("SELECT max(next_auid) FROM settings");
	adbquery.first();
	next_auid=adbquery.record().value(0).toString();
	adbquery.exec(qPrintable(	"UPDATE settings SET next_auid="+
								QString::number(next_auid.toInt()+1)+
								" WHERE next_auid="+next_auid));

	if (!adbquery.isActive())
		QMessageBox::warning(0, "aDatabase Error12",
				 adbquery.lastError().text());

	//copy route data
	adbquery.exec("ATTACH 'routes.db' AS rdb");
	if (!adbquery.isActive())
		QMessageBox::warning(0, "aDatabase Error13",
				 adbquery.lastError().text());
	cout << "next_auid: " << qPrintable(next_auid) << endl;
	adbquery.exec(qPrintable(	"CREATE TABLE route"+next_auid+
								" AS SELECT * FROM rdb.rroute"+uid));
	if (!adbquery.isActive())
		QMessageBox::warning(0, "aDatabase Error14",
				 adbquery.lastError().text());
	adbquery.exec("DETACH rdb");

	if (!adbquery.isActive())
		QMessageBox::warning(0, "aDatabase Error15",
				 adbquery.lastError().text());

	//copy route metadata
	QString values;
	for(int i = 0;i<21;i++) values.append("'"+metadata.value(i).toString()+"',");
		values.append("'"+metadata.value(21).toString()+"'");

	values.prepend(next_auid+",");
	cout << "values: " << qPrintable(values) << endl;
	adbquery.exec(qPrintable("INSERT INTO active_metadata VALUES("+values+")"));


	if (!adbquery.isActive()) {
		QMessageBox::warning(0, "aDatabase Error16",
				 adbquery.lastError().text());
		adbquery.finish(); }
	return next_auid;

}

//Returns now the UID under which the route was stored in rdb
QString saveRoute(QString auid)
{
    QString next_uid;
    QSqlQuery adbquery(QSqlDatabase::database("adb"));
    QSqlQuery rdbquery(QSqlDatabase::database("rdb"));

    qDebug("Saving route");

    //get metadata from the specific active route - klar soweit.
    QSqlRecord metadata;
    adbquery.exec(qPrintable("SELECT * FROM active_metadata WHERE auid="+auid));
    adbquery.first();
    metadata = adbquery.record();

    //todo rm messageboxes - wenn's scheiße läuft, blöde warnung aussgeben - klar soweit
    if (!adbquery.isActive()) QMessageBox::warning(0, "aDatabase Error30; Error code:"+QString::number(adbquery.lastError().number()),
						  "Database error: "+adbquery.lastError().databaseText()+
						  "\nDriver error: "+adbquery.lastError().driverText());

    adbquery.finish();

    //check if active route's uid is already present in rdb (-> existent or new)
    bool update;
    QString uid;

    if (metadata.value(1).toString() != "") uid=metadata.value(1).toString(); else uid="''";
    rdbquery.exec("SELECT * FROM metadata WHERE uid="+uid);

    update = rdbquery.next();


    if (!rdbquery.isActive()) QMessageBox::warning(0, "rDatabase Error2; Error code:"+QString::number(rdbquery.lastError().number()),
						   "Database error: "+rdbquery.lastError().databaseText()+
						   "\nDriver error: "+rdbquery.lastError().driverText());

    //what to do if the route is new:
    //find out the next uid for rdb and update it in rdb
    rdbquery.exec("SELECT max(next_uid) FROM rsettings");
    rdbquery.first();
    next_uid=rdbquery.record().value(0).toString();

    //some debug stuff
    qDebug("next_uid: "+next_uid);
    if(update) qDebug("update:");
    qDebug("auid:"+auid);
    qDebug("UID: " + uid);

    if( update )
    {
	rdbquery.exec(qPrintable("DROP Table rroute"+uid));
	next_uid = uid;
	if (!rdbquery.isActive())
	    QMessageBox::warning(0, "rDatabase Error32.1; Error code:"+QString::number(rdbquery.lastError().number()),
				 "Database error: "+rdbquery.lastError().databaseText()+	//unable to fetch row
				 "\nDriver error: "+rdbquery.lastError().driverText());	//database is locked
    }
    else
    {
	rdbquery.exec(qPrintable("UPDATE rsettings SET next_uid="+
				QString::number(next_uid.toInt()+1)
				+" WHERE next_uid="+next_uid));
    }

    //copy active route data to rdb
    adbquery.exec("ATTACH 'routes.db' AS rdb");

    if (!adbquery.isActive()) QMessageBox::warning(0, "aDatabase Error31; Error code:"+QString::number(adbquery.lastError().number()),
			     "Database error: "+adbquery.lastError().databaseText()+
			     "\nDriver error: "+adbquery.lastError().driverText());

    /*THIS WAS ADDED*/
    //The hard way: drop table before you add data from adb; TODO: check if there is a more gentle version


    adbquery.exec(qPrintable(	"CREATE TABLE rdb.rroute"+next_uid+
				" AS SELECT * FROM route"+auid));   //kopiere derzeitige Route von adb in Richtung rdb

    if (!adbquery.isActive())
	QMessageBox::warning(0, "aDatabase Error32; Error code:"+QString::number(adbquery.lastError().number()),
			     "Database error: "+adbquery.lastError().databaseText()+
			     "\nDriver error: "+adbquery.lastError().driverText());

    adbquery.exec("DETACH rdb");
    //QSqlDatabase::database("rdb").open();

    if (!adbquery.isActive())
	QMessageBox::warning(0, "aDatabase Error33; Error code:"+QString::number(adbquery.lastError().number()),
			     "Database error: "+adbquery.lastError().databaseText()+
			     "\nDriver error: "+adbquery.lastError().driverText());

    adbquery.finish();




    /*if (!rdbquery.isActive())
	QMessageBox::warning(0, "rDatabase Error4",
			     rdbquery.lastError().text());

    rdbquery.finish();*/


    //update metadata
    //format metadata values for query
    QString values;
    int i;
    for((i = 2);(i<22);(i++)) values.append("'"+metadata.value(i).toString()+"',");
    values.append("'"+metadata.value(22).toString()+"'");

    if (update)
    {
	//if it is an update, delete the old metadata entry and create a new one
	values.prepend(metadata.value(1).toString()+","); //uid
	//cout << "values: " << qPrintable(values) << endl;
	rdbquery.exec(qPrintable("DELETE FROM metadata WHERE uid="+metadata.value(1).toString()));
	//cout << "existent" << endl;
    }
    else
    {
	//if it is not an update, create a new metadata entry
	values.prepend(next_uid+",");
	//cout << "values: " << qPrintable(values) << endl;
	//cout << "new" << endl;
    }
    rdbquery.exec(qPrintable("INSERT INTO metadata VALUES("+values+")"));
    rdbquery.finish();

    return next_uid;

}

void deleteRoute(QString uid, QString database)
{
    int next_uid;
    //check if rdb route -> currently loaded
    QSqlQuery dbquery(QSqlDatabase::database(database));
    //delete route database entry
    dbquery.exec(qPrintable("DROP TABLE route"+uid));

    //delete metadata database entry
    //add uid to settings entry "empty"
    //if if uid == next_uid-1 next_uid--
    if (database == "adb") {

	dbquery.exec(qPrintable("DELETE FROM active_metadata WHERE auid="+uid));
	dbquery.exec("SELECT next_auid FROM settings");
	next_uid = dbquery.record().value(0).toInt();
	if (QString::number(next_uid-1) != uid) { dbquery.exec(qPrintable("INSERT INTO settings(empty) VALUES("+uid+")"));
	} else { dbquery.exec(qPrintable("UPDATE settings SET next_auid="+QString::number(next_uid-1)));}

    } else if (database == "rdb") {

	dbquery.exec(qPrintable("DELETE FROM metadata WHERE uid="+uid));
	dbquery.exec("SELECT next_uid FROM rsettings");
	next_uid = dbquery.record().value(0).toInt();
	if (QString::number(next_uid-1) != uid) { dbquery.exec(qPrintable("INSERT INTO rsettings(empty) VALUES("+uid+")"));
	} else { dbquery.exec(qPrintable("UPDATE rsettings SET next_uid="+QString::number(next_uid-1)));}

    }








    //afterwards update loaded route list
    //redraw map & trainer
}

void setMetadata(QString auid, QString data, QString type)
{
	QSqlQuery adbquery(QSqlDatabase::database("adb"));
	adbquery.exec(qPrintable("UPDATE active_metadata SET "+type+"="+data+" WHERE auid="+auid));
}

void setRouteName(QString auid, QString name) {setMetadata(auid, name.append("'").prepend("'"), "name");}
void setRouteTags(QString auid, QString tags) {setMetadata(auid, tags.append("'").prepend("'"), "tags");}
void setRouteID(QString auid, QString id) {setMetadata(auid, id, "id");}
void setRouteLocation(QString auid, QString location) {setMetadata(auid, location.append("'").prepend("'"), "location");}

void setRoutePicture(QString auid, QByteArray picture)
{
	QSqlQuery adbquery(QSqlDatabase::database("adb"));
	QString pictureString = QString::number(picture.toInt());
	adbquery.exec(qPrintable("UPDATE active_metadata SET picture="+pictureString));
}

void addTileToDB(int zoomLevel, int tileX, int tileY, QByteArray tile, QString mapsource)
{
    QString tileString = tile.toBase64();
    QSqlQuery mdbquery(QSqlDatabase::database("mdb"));
    mdbquery.exec(qPrintable(	"SELECT * FROM zoom"+QString::number(zoomLevel)+
				" WHERE xtile="+QString::number(tileX)+
				" AND ytile="+QString::number(tileY)+
				" AND mapsource="+mapsource.append("\"").prepend("\"")));
    if(!mdbquery.first())
    {
	mdbquery.exec(qPrintable(	"INSERT INTO zoom"+QString::number(zoomLevel)+" VALUES("
					+QString::number(tileX)+","
					+QString::number(tileY)+","
					+tileString.append("\"").prepend("\"")+","
					+mapsource+")"));
    }
    mdbquery.finish();
}

QByteArray getTileFromDB(int zoomLevel, int tileX, int tileY, QString mapsource)
{
	QSqlQuery mdbquery(QSqlDatabase::database("mdb"));
	mdbquery.exec(qPrintable(	"SELECT * FROM zoom"+QString::number(zoomLevel)+
								" WHERE xtile="+QString::number(tileX)+
								" AND ytile="+QString::number(tileY)+
								" AND mapsource="+mapsource.append("\"").prepend("\"")));
	if (mdbquery.first())
	{
	    QByteArray temparray;
	    QString tile = mdbquery.record().value(2).toString();
	    temparray = tile.toAscii();
	    QByteArray array = QByteArray::fromBase64(temparray);
	    //array.fromBase64(temparray);
	    tile = array.toBase64();
	    tile.truncate(10);
	    return array;
	}
	else return QString::number(0).toAscii();
}


