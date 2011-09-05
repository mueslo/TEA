#include <teaview.h>

TEAView::TEAView(QWidget *parent) : QGraphicsView(parent)
{

}

void TEAView::wheelEvent(QWheelEvent *event)
{
        int numDegrees = event->delta()/8;
        int numSteps = numDegrees/15;
        if (event->orientation() == Qt::Vertical)
        {
                emit wheelZoom(numSteps);
        }
}

void TEAView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    emit viewChanged();
}
