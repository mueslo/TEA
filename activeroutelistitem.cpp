#include "activeroutelistitem.h"
#include "tea.h"

ActiveRouteListItem::ActiveRouteListItem(const QString &name, int Auid, bool mod, QListWidget *parent) : QListWidgetItem(name,parent)
{
    colors << Qt::darkBlue << Qt::darkRed
           << Qt::darkGreen << Qt::darkCyan
           << Qt::darkMagenta << Qt::darkYellow;

    //initialise pointers to null
    curve = 0;
    path = 0;
    pathOutline = 0;

    setTextColor(Qt::black);
    setModified(mod);
    setName(name);
    setAuid(Auid);

    outlinePen.setBrush(Qt::white);
    setCheckState(Qt::Checked);
    setFlags( Qt::ItemIsEditable | flags() );
}

ActiveRouteListItem::~ActiveRouteListItem()
{
    //The QGraphicsScene deletes all items on it, and the QwtPlot too.
}

QGraphicsPathItem* ActiveRouteListItem::getPath()
{
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
    QColor color = colors.at(Auid%colors.count());
    setTextColor(color);
}

void ActiveRouteListItem::setModified(bool mod)
{
    /*
    m M = Action
    0 0 0 do Nothing
    1 0 1 append *
    0 1 0 chop *
    1 1 1 do nothing
    */

    if (mod!=Modified)
    {
     if (mod) setText(name+"*");
     else setText(name);
    }
    Modified = mod;
}

void ActiveRouteListItem::setName(QString newName)
{
    name = newName;
    if (Modified) newName.append("*");
    setText(newName);
}

QString ActiveRouteListItem::getName()
{
    return name;
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
    pen.setBrush(textColor());
    curve->setPen(pen);
    }
    return curve;
}
