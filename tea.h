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
    enum Save { ForceSave, AskToSave, DoNotSave };

public slots:

private slots:
    void downloaded(QNetworkReply* reply);
    void zoom(int steps);

    void sldChanged(int value);
    void viewChange();
    void rotateClockwise();
    void rotateCClockwise();
    void consoleButtonTriggered();
    void consoleChanged();
    void mapSourceChanged();
    void trainerModeChanged();
    void trainerSelectionChange();

    void removeRoute();
    void setGeneralSettings();

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

    /* Uninteresting functions */
    void exportSelectedTEA();
    void exportSelectedKML();
    void zoomIn();
    void zoomOut();
    void actionViewDatabase();
    void unloadAll();
    void unloadSelected();

    /* */
    //void graphicsViewResized();
    //void grphPressed();
    //void grphReleased();


private:
    Ui::MainWin ui;
    int getMetadata(QString auid);
    bool nodeNextSkip(QSqlQuery routeData, int timesToSkip);
    bool nodeNextSkip(QSqlQuery* routeData, int timesToSkip);
    bool maybeExit();


    void getTile(int tileX, int tileY, int zoomLevel);
    void getTilesInRange();

    void unload(ActiveRouteListItem* route);
    void unload(QList<QListWidgetItem*> routes);

    bool save(QList<QListWidgetItem*> routes, TEA::Save behaviour = TEA::AskToSave);
    bool save(ActiveRouteListItem *route, TEA::Save behaviour = TEA::AskToSave);

    void createStatusBar();

    void exportKML(QString auid);
    void exportSelected(QString format);
    void exportTEA(QString auid);

    void fillTrainerViewCBoxes();
    void connectSignalsAndSlots();

    void rerenderRoute();
    void addNewRoutes(QSqlQuery auidQuery, bool modified = false);
    void addRoute(QString auid, bool modified);
    void addPath(ActiveRouteListItem *route, QSqlQuery *routeData, QSqlRecord *metadata);
    void addCurve(ActiveRouteListItem *route, QSqlQuery *routeData, QSqlRecord *metadata);
    void redrawTrainer();
    void placeTile(QByteArray tile, int tileX, int tileY, int zoomLevel);
    void createToolBar();
    bool routeAdded(QString auid);
    bool routesModified();
    ActiveRouteListItem* find(QString auid);


    int zoomOld;
    QProgressBar *prgBar;
    QString mapSource;
    QNetworkAccessManager *networkManager;
    QStack<QNetworkRequest> requestStack;
    int currentRequests;
    QGraphicsScene *scene;
    bool init;

protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);


};

#endif // TEA_H
