/*
 * teaview.h
 *
 *  Created on: 02.01.2010
 *      Author: jf
 */

#ifndef TEAVIEW_H_
#define TEAVIEW_H_

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWheelEvent>


class TEAView : public QGraphicsView {
	Q_OBJECT

public:
	TEAView(QWidget *parent = 0);

signals:
	void wheelZoom(int);
        void viewChanged();

protected:
	void wheelEvent(QWheelEvent *event);
        virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif /* TEAVIEW_H_ */
