#ifndef TEA_H
#define TEA_H

#include <QtGui/QMainWindow>
#include "ui_tea.h"
#include <QFile>
#include <QtNetwork>
#include <QProgressBar>
#include <QtSql>
#include "activeroutelistitem.h"
class QFile;

class TEA : public QMainWindow
{
    Q_OBJECT

public:
    TEA(QWidget *parent = 0);
    ~TEA();

public slots:

private slots:
    void downloaded(QNetworkReply* reply);
    void zoom(int steps);
    void zoomIn();
    void zoomOut();
    void sldChanged(int value);
    void viewChange();
    void rotateClockwise();
    void rotateCClockwise();
    void consoleButtonTriggered();
    void consoleChanged();
    void mapSourceChanged();
    void trainerModeChanged();
    void trainerSelectionChange();
    void unloadRoute();
    void removeRoute();
    void setGeneralSettings();
    void actionViewDatabase();
    void updateADB();
    void updatePath(QListWidgetItem *Item);
    void editMetadata();
    void centerMapOnSelectedRoute();
    void saveAllToDatabase();
    void saveSelectedToDatabase();
    void loadFromFile();
    void loadFromDatabase();
    void saveToDatabase(QList<QListWidgetItem*> chosenItems);
    void About();
    void showListContextMenu(const QPoint &);
    //void graphicsViewResized();
    //void grphPressed();
    //void grphReleased();


private:
    Ui::MainWin ui;
    int getMetadata(QString auid);
    bool nodeNextSkip(QSqlQuery routeData, int timesToSkip);
    bool maybeExit();
    void drawTrainer(int indexX, int indexY);
    void getTile(int tileX, int tileY, int zoomLevel);
    void getTilesInRange();
    void createStatusBar();
    void fillTrainerViewCBoxes();
    void connectSignalsAndSlots();
    void rerenderRoute();
    void drawRoutes(QSqlQuery auidQuery);
    void drawRoute(QString auid, bool asterisk = true);
    void placeTile(QByteArray tile, int tileX, int tileY, int zoomLevel);
    void createToolBar();
    int zoomOld;
    QProgressBar *prgBar;
    QString mapSource;
    QFile *file;
    QNetworkAccessManager *networkManager;
    QGraphicsScene *scene;
    bool init;

protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);


};

#endif // TEA_H
