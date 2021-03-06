#include "tea.h"
#include <QtGui>
#include <QApplication>
#include "ui_tea.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
        QApplication::setGraphicsSystem("raster");
        QApplication a(argc, argv);
        QPixmap splashImage("splash.png");
        QSplashScreen splash(splashImage);
        splash.setMask(splashImage.mask());
        splash.show();
        a.setApplicationVersion("0.02");
	a.setApplicationName("TEA - Tour Evaluation and Analysis");
	TEA w;
	w.show();
        splash.finish(&w);
	return a.exec();
}
