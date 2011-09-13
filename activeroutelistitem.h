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
    explicit ActiveRouteListItem(const QString &newName, int newAuid, bool mod = 0, QListWidget *parent = 0);
    ~ActiveRouteListItem();

    QGraphicsPathItem* path() const;
    QGraphicsPathItem* pathOutline() const;
    QwtPlotCurve* curve() const;
    QString auid() const;
    QString name() const;
    bool isModified() const;

    void setName(QString newName);
    void setPath( QGraphicsPathItem *newPath); //TODO: rename setPath to setPathItem
    void setPathOutline(QGraphicsPathItem *newPathOutline);
    void setAuid(int newAuid);
    void setModified(bool mod=true);
    void setCurve(QwtPlotCurve *newCurve);


private:
    //Ui::MainWin ui;
    QGraphicsPathItem *Path;	//Pointer to PathItem in Memory (which is a child of QGraphicsView)
    QGraphicsPathItem *PathOutline;  //Pointer to PathItem in Memory
    QwtPlotCurve *Curve;

    QList<QColor> colors;
    int Auid;
    QString Name;
    bool Modified;

};

#endif // ACTIVEROUTELISTITEM_H
