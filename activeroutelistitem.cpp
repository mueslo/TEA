#include "activeroutelistitem.h"
#include "ui_tea.h"

ActiveRouteListItem::ActiveRouteListItem(const QString &text, int row) : QListWidgetItem(text)
{
   /* QListWidgetItem NewItem = new QListWidgetItem(text);
    NewItem->setCheckState(Qt::Checked);
    NewItem->setFlags( Qt::ItemIsEditable | NewItem->flags());
    ui.listWidget->insertItem(auid.toInt(), NewItem );*/

    this->setCheckState(Qt::Checked);
    this->setFlags( Qt::ItemIsEditable | this->flags() );
}

ActiveRouteListItem::~ActiveRouteListItem()
{
    ;
}

QGraphicsPathItem* ActiveRouteListItem::getPath()
{
    return path;
}

int ActiveRouteListItem::getAuid()
{
    return auid;
}

void ActiveRouteListItem::setPath( QGraphicsPathItem *Path)
{
   path = Path;
}

void ActiveRouteListItem::setAuid(int Auid)
{
    auid = Auid;
}
