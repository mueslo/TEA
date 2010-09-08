#ifndef PATHLIST_H
#define PATHLIST_H

#include <QListWidgetItem>
#include <QGraphicsPathItem>

//#include "tea.h"
#include <QtGui>


class PathList : public QObject, public QListWidgetItem
{
    Q_OBJECT

public:
    PathList(const QString &text, int row);
    QGraphicsPathItem* getPath();
    int getAuid();
    void setPath( QGraphicsPathItem *Path);
    void setAuid(int Auid);
    ~PathList();
private:
    //Ui::MainWin ui;
    QGraphicsPathItem *path;	//Pointer to PathItem in Memory (which is a child of QGraphicsView)
    int auid;

};

#endif // PATHLIST_H
