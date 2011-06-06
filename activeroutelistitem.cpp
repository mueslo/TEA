#include "activeroutelistitem.h"
#include "ui_tea.h"
#include "tea.h"

ActiveRouteListItem::ActiveRouteListItem(const QString &text, int row) : QListWidgetItem(text)
{
   /* QListWidgetItem NewItem = new QListWidgetItem(text);
    NewItem->setCheckState(Qt::Checked);
    NewItem->setFlags( Qt::ItemIsEditable | NewItem->flags());
    ui.listWidget->insertItem(auid.toInt(), NewItem );*/

    outlinePen.setBrush(Qt::white);

    Modified = false;
    this->setCheckState(Qt::Checked);
    this->setFlags( Qt::ItemIsEditable | this->flags() );

}

//TODO: implement QwtCurve in this class just like QPathItem with Path and Outline.

ActiveRouteListItem::~ActiveRouteListItem()
{
    ;
}

QGraphicsPathItem* ActiveRouteListItem::getPath()
{
    return path;
}

QGraphicsPathItem* ActiveRouteListItem::getPathOutline()
{
    pathOutline->setPen(outlinePen);
    return pathOutline;
}

void ActiveRouteListItem::setOutlineZoom(int zoomLevel)
{
   outlinePen.setWidthF(pow(2.0, -zoomLevel-2));
   pathOutline->setPen(outlinePen);
}

QString ActiveRouteListItem::getAuid()
{
    return QString::number(auid);
}

void ActiveRouteListItem::setPath( QGraphicsPathItem *Path)
{
   path = Path;
}

void ActiveRouteListItem::setPathOutline(QGraphicsPathItem *PathOutline)
{
    pathOutline = PathOutline;
    pathOutline->setPen(outlinePen);
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
