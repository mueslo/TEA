#include "pathlist.h"
#include "ui_tea.h"

PathList::PathList(const QString &text, int row) : QListWidgetItem(text)
{
   /* QListWidgetItem NewItem = new QListWidgetItem(text);
    NewItem->setCheckState(Qt::Checked);
    NewItem->setFlags( Qt::ItemIsEditable | NewItem->flags());
    ui.listWidget->insertItem(auid.toInt(), NewItem );*/

    this->setCheckState(Qt::Checked);
    this->setFlags( Qt::ItemIsEditable | this->flags() );
}

PathList::~PathList()
{
    ;
}

QGraphicsPathItem* PathList::getPath()
{
    return path;
}

int PathList::getAuid()
{
    return auid;
}

void PathList::setPath( QGraphicsPathItem *Path)
{
   path = Path;
}

void PathList::setAuid(int Auid)
{
    auid = Auid;
}
