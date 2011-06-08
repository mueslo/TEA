#include "activeroutelistitem.h"
#include "tea.h"

ActiveRouteListItem::ActiveRouteListItem(const QString &name, int auid) : QListWidgetItem(name)
{
   /* QListWidgetItem NewItem = new QListWidgetItem(text);
    NewItem->setCheckState(Qt::Checked);
    NewItem->setFlags( Qt::ItemIsEditable | NewItem->flags());
    ui.listWidget->insertItem(auid.toInt(), NewItem );*/

    //initialise pointers to null
    curve = 0;
    path = 0;
    pathOutline = 0;

    this->setTextColor(Qt::black);

    colors << Qt::darkBlue << Qt::darkRed
           << Qt::darkGreen << Qt::darkCyan
           << Qt::darkMagenta << Qt::darkYellow;

    outlinePen.setBrush(Qt::white);

    Modified = false;
    this->setCheckState(Qt::Checked);
    this->setFlags( Qt::ItemIsEditable | this->flags() );

}

//TODO: implement QwtCurve in this class just like QPathItem with Path and Outline.

ActiveRouteListItem::~ActiveRouteListItem()
{
    curve->detach();
    pathOutline->~QGraphicsPathItem();
    path->~QGraphicsPathItem();
    curve->~QwtPlotCurve();
}

QGraphicsPathItem* ActiveRouteListItem::getPath()
{
    //todo: set/create pen here, color is in TextColor
    if (path != 0)
    {
    QPen pen;
    pen.setBrush(this->textColor());
    path->setPen(pen);
    }
    return path;
}

QGraphicsPathItem* ActiveRouteListItem::getPathOutline()
{
    if (pathOutline != 0)
    {
    pathOutline->setPen(outlinePen);
    }

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
    this->setTextColor(colors.at(auid%colors.count()));
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

void ActiveRouteListItem::setCurve(QwtPlotCurve *Curve)
{
    curve=Curve;
}

QwtPlotCurve* ActiveRouteListItem::getCurve()
{
    if (curve != 0)
    {
    QPen pen;
    pen.setBrush(this->textColor());
    curve->setPen(pen);
    }
    return curve;
}
