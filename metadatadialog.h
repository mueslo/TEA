#ifndef METADATADIALOG_H
#define METADATADIALOG_H

#include <QtGui/QDialog>
#include "ui_metadatadialog.h"
#include <mapview.h>
#include <activeroutelistitem.h>
#include <QSqlQuery>

class MetadataDialog : public QDialog
{
    Q_OBJECT

public:
    MetadataDialog(QString auid, int type = 0, QWidget *parent = 0);
    ~MetadataDialog();
    void fillKnownMetadata();

private slots:
	void cancel();
	void save();
	void enableLoadButton(const QString &text);

private:
	Ui::MetadataDialogClass ui;
	void connectSignalsAndSlots();
	void renderRoute();
	bool nodeNextSkip(QSqlQuery routeData, int timesToSkip);
        ActiveRouteListItem* route;
	QPolygon mpRoute;
	QString mpAuid;

protected:
        virtual void resizeEvent(QResizeEvent *event);
    };

#endif // METADATADIALOG_H
