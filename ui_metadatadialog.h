/********************************************************************************
** Form generated from reading UI file 'metadatadialog.ui'
**
** Created: Thu 18. Mar 13:13:57 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_METADATADIALOG_H
#define UI_METADATADIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCalendarWidget>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGraphicsView>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QTimeEdit>

QT_BEGIN_NAMESPACE

class Ui_MetadataDialogClass
{
public:
    QDialogButtonBox *btnbResponse;
    QGroupBox *grpGeneral;
    QLabel *lblName;
    QLineEdit *edtName;
    QLineEdit *edtLocation;
    QLabel *lblLocation;
    QPlainTextEdit *edtTags;
    QLabel *lblTags;
    QLineEdit *edtID;
    QLabel *label;
    QGroupBox *grpTime;
    QCalendarWidget *calDate;
    QTimeEdit *tedtDuration;
    QLabel *lblDuration;
    QGroupBox *grpVisual;
    QGraphicsView *graphRoute;
    QSlider *verticalSlider;
    QPushButton *btnZoomIn;
    QPushButton *btnZoomOut;
    QGroupBox *groupBox;
    QLabel *lblTotalAltitudeDifference;
    QLabel *lblAltitudeGain;
    QLabel *lblAltitudeLoss;
    QLabel *lblDistance;
    QLabel *lblAvgVelocity;
    QLabel *lblAvgAltitude;

    void setupUi(QDialog *MetadataDialogClass)
    {
        if (MetadataDialogClass->objectName().isEmpty())
            MetadataDialogClass->setObjectName(QString::fromUtf8("MetadataDialogClass"));
        MetadataDialogClass->resize(690, 480);
        btnbResponse = new QDialogButtonBox(MetadataDialogClass);
        btnbResponse->setObjectName(QString::fromUtf8("btnbResponse"));
        btnbResponse->setGeometry(QRect(515, 445, 156, 23));
        btnbResponse->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);
        grpGeneral = new QGroupBox(MetadataDialogClass);
        grpGeneral->setObjectName(QString::fromUtf8("grpGeneral"));
        grpGeneral->setGeometry(QRect(10, 10, 231, 211));
        QFont font;
        font.setPointSize(10);
        grpGeneral->setFont(font);
        lblName = new QLabel(grpGeneral);
        lblName->setObjectName(QString::fromUtf8("lblName"));
        lblName->setGeometry(QRect(10, 32, 46, 14));
        QFont font1;
        font1.setPointSize(8);
        lblName->setFont(font1);
        edtName = new QLineEdit(grpGeneral);
        edtName->setObjectName(QString::fromUtf8("edtName"));
        edtName->setGeometry(QRect(110, 30, 113, 20));
        edtName->setFont(font1);
        edtLocation = new QLineEdit(grpGeneral);
        edtLocation->setObjectName(QString::fromUtf8("edtLocation"));
        edtLocation->setGeometry(QRect(110, 60, 113, 20));
        edtLocation->setFont(font1);
        lblLocation = new QLabel(grpGeneral);
        lblLocation->setObjectName(QString::fromUtf8("lblLocation"));
        lblLocation->setGeometry(QRect(10, 62, 46, 14));
        lblLocation->setFont(font1);
        edtTags = new QPlainTextEdit(grpGeneral);
        edtTags->setObjectName(QString::fromUtf8("edtTags"));
        edtTags->setGeometry(QRect(110, 120, 111, 71));
        edtTags->setFont(font1);
        lblTags = new QLabel(grpGeneral);
        lblTags->setObjectName(QString::fromUtf8("lblTags"));
        lblTags->setGeometry(QRect(10, 122, 46, 14));
        lblTags->setFont(font1);
        edtID = new QLineEdit(grpGeneral);
        edtID->setObjectName(QString::fromUtf8("edtID"));
        edtID->setGeometry(QRect(110, 90, 113, 20));
        label = new QLabel(grpGeneral);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 92, 46, 14));
        label->setFont(font1);
        grpTime = new QGroupBox(MetadataDialogClass);
        grpTime->setObjectName(QString::fromUtf8("grpTime"));
        grpTime->setGeometry(QRect(10, 230, 231, 231));
        grpTime->setFont(font);
        calDate = new QCalendarWidget(grpTime);
        calDate->setObjectName(QString::fromUtf8("calDate"));
        calDate->setGeometry(QRect(10, 60, 211, 162));
        QPalette palette;
        QBrush brush(QColor(51, 153, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::HighlightedText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush);
        palette.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush);
        palette.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush1);
        calDate->setPalette(palette);
        calDate->setFont(font1);
        calDate->setFirstDayOfWeek(Qt::Monday);
        calDate->setGridVisible(true);
        calDate->setSelectionMode(QCalendarWidget::NoSelection);
        calDate->setHorizontalHeaderFormat(QCalendarWidget::ShortDayNames);
        calDate->setVerticalHeaderFormat(QCalendarWidget::ISOWeekNumbers);
        calDate->setDateEditEnabled(false);
        tedtDuration = new QTimeEdit(grpTime);
        tedtDuration->setObjectName(QString::fromUtf8("tedtDuration"));
        tedtDuration->setGeometry(QRect(110, 30, 111, 22));
        tedtDuration->setFont(font1);
        tedtDuration->setReadOnly(true);
        tedtDuration->setButtonSymbols(QAbstractSpinBox::NoButtons);
        lblDuration = new QLabel(grpTime);
        lblDuration->setObjectName(QString::fromUtf8("lblDuration"));
        lblDuration->setGeometry(QRect(20, 32, 46, 14));
        lblDuration->setFont(font1);
        grpVisual = new QGroupBox(MetadataDialogClass);
        grpVisual->setObjectName(QString::fromUtf8("grpVisual"));
        grpVisual->setGeometry(QRect(260, 10, 421, 301));
        grpVisual->setFont(font);
        graphRoute = new QGraphicsView(grpVisual);
        graphRoute->setObjectName(QString::fromUtf8("graphRoute"));
        graphRoute->setGeometry(QRect(10, 20, 371, 271));
        graphRoute->setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::TextAntialiasing);
        verticalSlider = new QSlider(grpVisual);
        verticalSlider->setObjectName(QString::fromUtf8("verticalSlider"));
        verticalSlider->setGeometry(QRect(390, 80, 19, 160));
        verticalSlider->setOrientation(Qt::Vertical);
        btnZoomIn = new QPushButton(grpVisual);
        btnZoomIn->setObjectName(QString::fromUtf8("btnZoomIn"));
        btnZoomIn->setGeometry(QRect(388, 50, 23, 23));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/16x16_0560/magnifier_zoom_in.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnZoomIn->setIcon(icon);
        btnZoomOut = new QPushButton(grpVisual);
        btnZoomOut->setObjectName(QString::fromUtf8("btnZoomOut"));
        btnZoomOut->setGeometry(QRect(388, 250, 23, 23));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/16x16_0560/magnifier_zoom_out.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnZoomOut->setIcon(icon1);
        groupBox = new QGroupBox(MetadataDialogClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(260, 320, 421, 111));
        groupBox->setFont(font);
        lblTotalAltitudeDifference = new QLabel(groupBox);
        lblTotalAltitudeDifference->setObjectName(QString::fromUtf8("lblTotalAltitudeDifference"));
        lblTotalAltitudeDifference->setGeometry(QRect(10, 50, 221, 16));
        lblTotalAltitudeDifference->setFont(font1);
        lblAltitudeGain = new QLabel(groupBox);
        lblAltitudeGain->setObjectName(QString::fromUtf8("lblAltitudeGain"));
        lblAltitudeGain->setGeometry(QRect(10, 70, 171, 16));
        lblAltitudeGain->setFont(font1);
        lblAltitudeLoss = new QLabel(groupBox);
        lblAltitudeLoss->setObjectName(QString::fromUtf8("lblAltitudeLoss"));
        lblAltitudeLoss->setGeometry(QRect(200, 70, 121, 20));
        lblAltitudeLoss->setFont(font1);
        lblDistance = new QLabel(groupBox);
        lblDistance->setObjectName(QString::fromUtf8("lblDistance"));
        lblDistance->setGeometry(QRect(10, 30, 261, 16));
        lblDistance->setFont(font1);
        lblAvgVelocity = new QLabel(groupBox);
        lblAvgVelocity->setObjectName(QString::fromUtf8("lblAvgVelocity"));
        lblAvgVelocity->setGeometry(QRect(10, 90, 181, 16));
        lblAvgVelocity->setFont(font1);
        lblAvgAltitude = new QLabel(groupBox);
        lblAvgAltitude->setObjectName(QString::fromUtf8("lblAvgAltitude"));
        lblAvgAltitude->setGeometry(QRect(200, 90, 211, 16));
        lblAvgAltitude->setFont(font1);

        retranslateUi(MetadataDialogClass);

        QMetaObject::connectSlotsByName(MetadataDialogClass);
    } // setupUi

    void retranslateUi(QDialog *MetadataDialogClass)
    {
        MetadataDialogClass->setWindowTitle(QApplication::translate("MetadataDialogClass", "Route Information", 0, QApplication::UnicodeUTF8));
        grpGeneral->setTitle(QApplication::translate("MetadataDialogClass", "General Information", 0, QApplication::UnicodeUTF8));
        lblName->setText(QApplication::translate("MetadataDialogClass", "Name:", 0, QApplication::UnicodeUTF8));
        lblLocation->setText(QApplication::translate("MetadataDialogClass", "Location:", 0, QApplication::UnicodeUTF8));
        edtTags->setPlainText(QString());
        lblTags->setText(QApplication::translate("MetadataDialogClass", "Tags:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MetadataDialogClass", "ID:", 0, QApplication::UnicodeUTF8));
        grpTime->setTitle(QApplication::translate("MetadataDialogClass", "Time", 0, QApplication::UnicodeUTF8));
        lblDuration->setText(QApplication::translate("MetadataDialogClass", "Duration:", 0, QApplication::UnicodeUTF8));
        grpVisual->setTitle(QApplication::translate("MetadataDialogClass", "Route", 0, QApplication::UnicodeUTF8));
        btnZoomIn->setText(QString());
        btnZoomOut->setText(QString());
        groupBox->setTitle(QApplication::translate("MetadataDialogClass", "Statistics", 0, QApplication::UnicodeUTF8));
        lblTotalAltitudeDifference->setText(QApplication::translate("MetadataDialogClass", "Total altitude difference:", 0, QApplication::UnicodeUTF8));
        lblAltitudeGain->setText(QApplication::translate("MetadataDialogClass", "Altitude gain:", 0, QApplication::UnicodeUTF8));
        lblAltitudeLoss->setText(QApplication::translate("MetadataDialogClass", "Altitude loss:", 0, QApplication::UnicodeUTF8));
        lblDistance->setText(QApplication::translate("MetadataDialogClass", "Distance:", 0, QApplication::UnicodeUTF8));
        lblAvgVelocity->setText(QApplication::translate("MetadataDialogClass", "Average velocity:", 0, QApplication::UnicodeUTF8));
        lblAvgAltitude->setText(QApplication::translate("MetadataDialogClass", "Average altitude:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MetadataDialogClass: public Ui_MetadataDialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_METADATADIALOG_H
