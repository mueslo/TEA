#include "activeroutelistitem.h"
#include "ui_tea.h"
#include "tea.h"

ActiveRouteListItem::ActiveRouteListItem(const QString &text, int row) : QListWidgetItem(text)
{
   /* QListWidgetItem NewItem = new QListWidgetItem(text);
    NewItem->setCheckState(Qt::Checked);
    NewItem->setFlags( Qt::ItemIsEditable | NewItem->flags());
    ui.listWidget->insertItem(auid.toInt(), NewItem );*/

    Modified = false;
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

void ActiveRouteListItem::setModified(bool mod)
{
    if(mod)
    {
	if(!Modified)
	{
	    Modified = true;
	    setText(text().append("*"));
	}
    }else
    {
	if(Modified)
	{
	    Modified = false;
	    QString temp = text();
	    temp.chop(1);
	    setText(temp);
	}
    }
}

bool ActiveRouteListItem::isModified()
{
    return Modified;
}
