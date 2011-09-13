#ifndef TEA_H
#define TEA_H

#include <QtGui/QMainWindow>
#include "ui_tea.h"
#include <QFile>
#include <QtNetwork>
#include <QProgressBar>
#include <QtSql>
#include "activeroutelistitem.h"
#include <QObject>

class TEA : public QMainWindow
{
    Q_OBJECT

public:
    TEA(QWidget *parent = 0);
    virtual ~TEA();
    enum Save { ForceSave, AskToSave, DoNotSave };

public slots:

private slots:

    //MapView
    void zoomChanged(int zoomLevel);
    void sldChanged(int zoomLevel);
    void mapSourceChanged(int minZoom, int maxZoom);
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
    void loadFromFile(QString TEAFilePath = "");
    void loadFromDatabase();
    void saveToDatabase(QList<QListWidgetItem*> chosenItems);

    void About();
    void showListContextMenu(const QPoint &);

    void settings();

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
    void createToolBar();
    bool routeAdded(QString auid);
    bool routesModified();
    ActiveRouteListItem* find(QString auid);

    QProgressBar *prgBar;
    QString mapSource;

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);


};

#endif // TEA_H
