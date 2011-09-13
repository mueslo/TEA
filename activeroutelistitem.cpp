#include "activeroutelistitem.h"
#include "tea.h"

ActiveRouteListItem::ActiveRouteListItem(const QString &newName, int newAuid, bool mod, QListWidget *parent) : QListWidgetItem(newName,parent)
{
    colors << Qt::darkBlue << Qt::darkRed
           << Qt::darkGreen << Qt::darkCyan
           << Qt::darkMagenta << Qt::darkYellow;

    //initialise pointers to null
    Curve = 0;
    Path = 0;
    PathOutline = 0;

    setTextColor(Qt::black);
    setModified(mod);
    setName(newName);
    setAuid(newAuid);

    setCheckState(Qt::Checked);
    setFlags( Qt::ItemIsEditable | flags() );
}

ActiveRouteListItem::~ActiveRouteListItem()
{
    //The QGraphicsScene deletes all items on it, and the QwtPlot too.

}

QGraphicsPathItem* ActiveRouteListItem::path() const
{
    if (Path != 0)
    {
    QPen pen;
    pen.setBrush(this->textColor());
    Path->setPen(pen);
    }
    return Path;
}

QGraphicsPathItem* ActiveRouteListItem::pathOutline() const
{
    return PathOutline;
}

QString ActiveRouteListItem::auid() const
{
    return QString::number(Auid);
}

void ActiveRouteListItem::setPath( QGraphicsPathItem *newPath)
{
   Path = newPath;
}

void ActiveRouteListItem::setPathOutline(QGraphicsPathItem *newPathOutline)
{
    PathOutline = newPathOutline;
}

void ActiveRouteListItem::setAuid(int newAuid)
{
    Auid = newAuid;
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
     if (mod) setText(Name+"*");
     else setText(Name);
    }
    Modified = mod;
}

void ActiveRouteListItem::setName(QString newName)
{
    Name = newName;
    if (Modified) newName.append("*");
    setText(newName);
}

QString ActiveRouteListItem::name() const
{
    return Name;
}

bool ActiveRouteListItem::isModified() const
{
    return Modified;
}

void ActiveRouteListItem::setCurve(QwtPlotCurve *newCurve)
{
    Curve=newCurve;
}

QwtPlotCurve* ActiveRouteListItem::curve() const
{
    if (Curve != 0)
    {
    QPen pen;
    pen.setBrush(textColor());
    Curve->setPen(pen);
    }
    return Curve;
}
