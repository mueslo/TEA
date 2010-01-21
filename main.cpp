#include "tea.h"
#include <QtGui>
#include <QApplication>
#include "ui_tea.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationVersion("0.1");
	a.setApplicationName("TEA - Tour Evaluation and Analysis");

	TEA w;
	w.show();
	return a.exec();
}
