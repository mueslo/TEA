#ifndef ACTIVEROUTELISTITEM_H
#define ACTIVEROUTELISTITEM_H

#include <QListWidgetItem>
#include <QGraphicsPathItem>

//#include "tea.h"
#include <QtGui>


class ActiveRouteListItem : public QObject, public QListWidgetItem
{
    Q_OBJECT

public:
    ActiveRouteListItem(const QString &text, int row);
    QGraphicsPathItem* getPath();
    int getAuid();
    void setPath( QGraphicsPathItem *Path);
    void setAuid(int Auid);
    ~ActiveRouteListItem();
    void setModified(bool mod=1);
    bool isModified();

private:
    //Ui::MainWin ui;
    QGraphicsPathItem *path;	//Pointer to PathItem in Memory (which is a child of QGraphicsView)
    int auid;
    bool Modified;

};

#endif // ACTIVEROUTELISTITEM_H
