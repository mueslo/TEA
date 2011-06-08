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
    ActiveRouteListItem(const QString &name, int row);
    QGraphicsPathItem* getPath();
    QGraphicsPathItem* getPathOutline();
    QString getAuid();
    void setOutlineZoom(int zoomLevel);
    void setPath( QGraphicsPathItem *Path); //TODO: rename setPath to setPathItem
    void setPathOutline(QGraphicsPathItem *PathOutline);
    void setAuid(int Auid);
    ~ActiveRouteListItem();
    void setModified(bool mod=1);
    bool isModified();
    void setCurve(QwtPlotCurve *Curve);
    QwtPlotCurve* getCurve();

private:
    //Ui::MainWin ui;
    QGraphicsPathItem *path;	//Pointer to PathItem in Memory (which is a child of QGraphicsView)
    QGraphicsPathItem *pathOutline;  //Pointer to PathItem in Memory
    QPen routePen;
    QPen outlinePen;
    QwtPlotCurve *curve;
    QList<QColor> colors;
    int auid;
    bool Modified;

};

#endif // ACTIVEROUTELISTITEM_H
