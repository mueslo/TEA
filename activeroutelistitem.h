#ifndef ACTIVEROUTELISTITEM_H
#define ACTIVEROUTELISTITEM_H

#include <QListWidgetItem>
#include <QGraphicsPathItem>

#include "qwt_plot_curve.h"
//#include "tea.h"
#include <QtGui>


class ActiveRouteListItem : public QObject, public QListWidgetItem
{
    Q_OBJECT

public:
    explicit ActiveRouteListItem(const QString &name, int auid, bool mod = 0, QListWidget *parent = 0);
    ~ActiveRouteListItem();

    QGraphicsPathItem* getPath();
    QGraphicsPathItem* getPathOutline();
    QwtPlotCurve* getCurve();
    QString getAuid();
    QString getName();
    bool isModified();

    void setOutlineZoom(int zoomLevel);
    void setName(QString newName);
    void setPath( QGraphicsPathItem *Path); //TODO: rename setPath to setPathItem
    void setPathOutline(QGraphicsPathItem *PathOutline);
    void setAuid(int Auid);
    void setModified(bool mod=true);
    void setCurve(QwtPlotCurve *Curve);


private:
    //Ui::MainWin ui;
    QGraphicsPathItem *path;	//Pointer to PathItem in Memory (which is a child of QGraphicsView)
    QGraphicsPathItem *pathOutline;  //Pointer to PathItem in Memory
    QwtPlotCurve *curve;
    QPen outlinePen;

    QList<QColor> colors;
    int auid;
    QString name;
    bool Modified;

};

#endif // ACTIVEROUTELISTITEM_H
