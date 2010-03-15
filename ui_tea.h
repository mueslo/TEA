/********************************************************************************
** Form generated from reading UI file 'tea.ui'
**
** Created: Mon 15. Mar 10:38:36 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEA_H
#define UI_TEA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QDockWidget>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <teaview.h>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWin
{
public:
    QAction *loadFromFileAction;
    QAction *loadFromDatabaseAction;
    QAction *exitAction;
    QAction *aboutAction;
    QAction *consoleAction;
    QAction *saveAction;
    QAction *saveAllAction;
    QAction *refreshMapAction;
    QAction *kmlExportAction;
    QAction *actionClose_route;
    QAction *actionDelete_route_from_database;
    QAction *actionEdit_metadata;
    QAction *actionExport_as_tea;
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QTabWidget *tabwCentre;
    QWidget *tabMap;
    QGridLayout *gridLayout_4;
    QVBoxLayout *layoutZoom;
    QPushButton *btnZoomIn;
    QSlider *sldZoom;
    QPushButton *btnZoomOut;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    TEAView *graphicsView;
    QHBoxLayout *horizontalLayout;
    QCommandLinkButton *cbtnRotateCClockwise;
    QCommandLinkButton *cbtnRotateClockwise;
    QWidget *tabTrainer;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QComboBox *cboxY;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QComboBox *cboxX;
    QwtPlot *qwtPlot;
    QMenuBar *menubar;
    QMenu *menuRoute;
    QMenu *menuHelp;
    QMenu *menuView;
    QStatusBar *statusbar;
    QToolBar *tbMain;
    QDockWidget *dwBottom;
    QWidget *dockWidgetContents_18;
    QGridLayout *gridLayout;
    QTextEdit *textInformation;
    QDockWidget *dwActiveRoutes;
    QWidget *dockWidgetContents_14;
    QGridLayout *gridLayout_2;
    QListWidget *listWidget;
    QDockWidget *dwSettingsControls;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *grpbMapSource;
    QFormLayout *formLayout;
    QRadioButton *rbMapnik;
    QRadioButton *rbCycleMap;
    QGroupBox *grpbTrainerMode;
    QFormLayout *formLayout_2;
    QRadioButton *rbNode;
    QRadioButton *rbRoute;
    QButtonGroup *btngrpMapSource;
    QButtonGroup *btngrpTrainerMode;

    void setupUi(QMainWindow *MainWin)
    {
        if (MainWin->objectName().isEmpty())
            MainWin->setObjectName(QString::fromUtf8("MainWin"));
        MainWin->resize(1024, 810);
        QIcon icon;
        icon.addFile(QString::fromUtf8("icons/32x32_0320/cup.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWin->setWindowIcon(icon);
        loadFromFileAction = new QAction(MainWin);
        loadFromFileAction->setObjectName(QString::fromUtf8("loadFromFileAction"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("icons/32x32_0320/cup_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        loadFromFileAction->setIcon(icon1);
        loadFromDatabaseAction = new QAction(MainWin);
        loadFromDatabaseAction->setObjectName(QString::fromUtf8("loadFromDatabaseAction"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("icons/32x32_0320/database_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        loadFromDatabaseAction->setIcon(icon2);
        exitAction = new QAction(MainWin);
        exitAction->setObjectName(QString::fromUtf8("exitAction"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("release/icons/32x32_0340/door.png"), QSize(), QIcon::Normal, QIcon::Off);
        exitAction->setIcon(icon3);
        aboutAction = new QAction(MainWin);
        aboutAction->setObjectName(QString::fromUtf8("aboutAction"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("icons/32x32_0480/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        aboutAction->setIcon(icon4);
        consoleAction = new QAction(MainWin);
        consoleAction->setObjectName(QString::fromUtf8("consoleAction"));
        consoleAction->setCheckable(true);
        consoleAction->setChecked(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/icons/16x16_0040/application_xp_terminal.png"), QSize(), QIcon::Normal, QIcon::Off);
        consoleAction->setIcon(icon5);
        consoleAction->setVisible(true);
        saveAction = new QAction(MainWin);
        saveAction->setObjectName(QString::fromUtf8("saveAction"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("icons/32x32_0340/disk.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveAction->setIcon(icon6);
        saveAllAction = new QAction(MainWin);
        saveAllAction->setObjectName(QString::fromUtf8("saveAllAction"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8("icons/32x32_0340/disk_multiple.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveAllAction->setIcon(icon7);
        refreshMapAction = new QAction(MainWin);
        refreshMapAction->setObjectName(QString::fromUtf8("refreshMapAction"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8("icons/32x32_0560/map.png"), QSize(), QIcon::Normal, QIcon::Off);
        refreshMapAction->setIcon(icon8);
        kmlExportAction = new QAction(MainWin);
        kmlExportAction->setObjectName(QString::fromUtf8("kmlExportAction"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8("icons/32x32_0640/page_save.png"), QSize(), QIcon::Normal, QIcon::Off);
        kmlExportAction->setIcon(icon9);
        actionClose_route = new QAction(MainWin);
        actionClose_route->setObjectName(QString::fromUtf8("actionClose_route"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8("icons/32x32_0320/cup_delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose_route->setIcon(icon10);
        actionDelete_route_from_database = new QAction(MainWin);
        actionDelete_route_from_database->setObjectName(QString::fromUtf8("actionDelete_route_from_database"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8("icons/32x32_0320/database_delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete_route_from_database->setIcon(icon11);
        actionEdit_metadata = new QAction(MainWin);
        actionEdit_metadata->setObjectName(QString::fromUtf8("actionEdit_metadata"));
        actionExport_as_tea = new QAction(MainWin);
        actionExport_as_tea->setObjectName(QString::fromUtf8("actionExport_as_tea"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8("icons/32x32_0640/page_white_cup.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExport_as_tea->setIcon(icon12);
        centralwidget = new QWidget(MainWin);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_3 = new QGridLayout(centralwidget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tabwCentre = new QTabWidget(centralwidget);
        tabwCentre->setObjectName(QString::fromUtf8("tabwCentre"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabwCentre->sizePolicy().hasHeightForWidth());
        tabwCentre->setSizePolicy(sizePolicy);
        tabwCentre->setTabsClosable(false);
        tabwCentre->setMovable(true);
        tabMap = new QWidget();
        tabMap->setObjectName(QString::fromUtf8("tabMap"));
        sizePolicy.setHeightForWidth(tabMap->sizePolicy().hasHeightForWidth());
        tabMap->setSizePolicy(sizePolicy);
        gridLayout_4 = new QGridLayout(tabMap);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        layoutZoom = new QVBoxLayout();
        layoutZoom->setObjectName(QString::fromUtf8("layoutZoom"));
        btnZoomIn = new QPushButton(tabMap);
        btnZoomIn->setObjectName(QString::fromUtf8("btnZoomIn"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btnZoomIn->sizePolicy().hasHeightForWidth());
        btnZoomIn->setSizePolicy(sizePolicy1);
        btnZoomIn->setMaximumSize(QSize(23, 23));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8("icons/16x16_0560/magnifier_zoom_in.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnZoomIn->setIcon(icon13);

        layoutZoom->addWidget(btnZoomIn);

        sldZoom = new QSlider(tabMap);
        sldZoom->setObjectName(QString::fromUtf8("sldZoom"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sldZoom->sizePolicy().hasHeightForWidth());
        sldZoom->setSizePolicy(sizePolicy2);
        sldZoom->setMinimumSize(QSize(23, 115));
        sldZoom->setMaximumSize(QSize(23, 16777215));
        sldZoom->setMaximum(18);
        sldZoom->setPageStep(1);
        sldZoom->setTracking(true);
        sldZoom->setOrientation(Qt::Vertical);
        sldZoom->setInvertedAppearance(false);
        sldZoom->setInvertedControls(false);

        layoutZoom->addWidget(sldZoom);

        btnZoomOut = new QPushButton(tabMap);
        btnZoomOut->setObjectName(QString::fromUtf8("btnZoomOut"));
        sizePolicy1.setHeightForWidth(btnZoomOut->sizePolicy().hasHeightForWidth());
        btnZoomOut->setSizePolicy(sizePolicy1);
        btnZoomOut->setMaximumSize(QSize(23, 23));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8("icons/16x16_0560/magnifier_zoom_out.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnZoomOut->setIcon(icon14);

        layoutZoom->addWidget(btnZoomOut);


        gridLayout_4->addLayout(layoutZoom, 0, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetNoConstraint);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        graphicsView = new TEAView(tabMap);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setMinimumSize(QSize(400, 300));
        graphicsView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::CrossCursor)));
        graphicsView->setMouseTracking(true);
        graphicsView->setFrameShape(QFrame::StyledPanel);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);
        graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

        verticalLayout->addWidget(graphicsView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cbtnRotateCClockwise = new QCommandLinkButton(tabMap);
        cbtnRotateCClockwise->setObjectName(QString::fromUtf8("cbtnRotateCClockwise"));
        sizePolicy1.setHeightForWidth(cbtnRotateCClockwise->sizePolicy().hasHeightForWidth());
        cbtnRotateCClockwise->setSizePolicy(sizePolicy1);
        cbtnRotateCClockwise->setMaximumSize(QSize(36, 36));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/icons/icons/32x32_0060/arrow_rotate_anticlockwise.png"), QSize(), QIcon::Normal, QIcon::Off);
        cbtnRotateCClockwise->setIcon(icon15);
        cbtnRotateCClockwise->setCheckable(false);
        cbtnRotateCClockwise->setAutoRepeat(false);

        horizontalLayout->addWidget(cbtnRotateCClockwise);

        cbtnRotateClockwise = new QCommandLinkButton(tabMap);
        cbtnRotateClockwise->setObjectName(QString::fromUtf8("cbtnRotateClockwise"));
        sizePolicy1.setHeightForWidth(cbtnRotateClockwise->sizePolicy().hasHeightForWidth());
        cbtnRotateClockwise->setSizePolicy(sizePolicy1);
        cbtnRotateClockwise->setMaximumSize(QSize(36, 36));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8("icons/32x32_0060/arrow_rotate_clockwise.png"), QSize(), QIcon::Normal, QIcon::Off);
        cbtnRotateClockwise->setIcon(icon16);

        horizontalLayout->addWidget(cbtnRotateClockwise);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);


        gridLayout_4->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        QIcon icon17;
        icon17.addFile(QString::fromUtf8("icons/16x16_0560/map.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabwCentre->addTab(tabMap, icon17, QString());
        tabTrainer = new QWidget();
        tabTrainer->setObjectName(QString::fromUtf8("tabTrainer"));
        gridLayout_6 = new QGridLayout(tabTrainer);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        cboxY = new QComboBox(tabTrainer);
        cboxY->setObjectName(QString::fromUtf8("cboxY"));
        sizePolicy1.setHeightForWidth(cboxY->sizePolicy().hasHeightForWidth());
        cboxY->setSizePolicy(sizePolicy1);

        gridLayout_5->addWidget(cboxY, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        cboxX = new QComboBox(tabTrainer);
        cboxX->setObjectName(QString::fromUtf8("cboxX"));
        sizePolicy1.setHeightForWidth(cboxX->sizePolicy().hasHeightForWidth());
        cboxX->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(cboxX);


        gridLayout_5->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        qwtPlot = new QwtPlot(tabTrainer);
        qwtPlot->setObjectName(QString::fromUtf8("qwtPlot"));

        gridLayout_5->addWidget(qwtPlot, 1, 0, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);

        QIcon icon18;
        icon18.addFile(QString::fromUtf8("icons/16x16_0220/chart_curve.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabwCentre->addTab(tabTrainer, icon18, QString());

        gridLayout_3->addWidget(tabwCentre, 0, 0, 1, 1);

        MainWin->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWin);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1024, 23));
        menuRoute = new QMenu(menubar);
        menuRoute->setObjectName(QString::fromUtf8("menuRoute"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWin->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWin);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWin->setStatusBar(statusbar);
        tbMain = new QToolBar(MainWin);
        tbMain->setObjectName(QString::fromUtf8("tbMain"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(tbMain->sizePolicy().hasHeightForWidth());
        tbMain->setSizePolicy(sizePolicy3);
        tbMain->setAllowedAreas(Qt::AllToolBarAreas);
        tbMain->setIconSize(QSize(24, 24));
        MainWin->addToolBar(Qt::TopToolBarArea, tbMain);
        dwBottom = new QDockWidget(MainWin);
        dwBottom->setObjectName(QString::fromUtf8("dwBottom"));
        sizePolicy1.setHeightForWidth(dwBottom->sizePolicy().hasHeightForWidth());
        dwBottom->setSizePolicy(sizePolicy1);
        dwBottom->setMinimumSize(QSize(406, 236));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8("icons/32x32_0040/application_xp_terminal.png"), QSize(), QIcon::Normal, QIcon::Off);
        dwBottom->setWindowIcon(icon19);
        dwBottom->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dwBottom->setAllowedAreas(Qt::BottomDockWidgetArea);
        dockWidgetContents_18 = new QWidget();
        dockWidgetContents_18->setObjectName(QString::fromUtf8("dockWidgetContents_18"));
        sizePolicy.setHeightForWidth(dockWidgetContents_18->sizePolicy().hasHeightForWidth());
        dockWidgetContents_18->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(dockWidgetContents_18);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textInformation = new QTextEdit(dockWidgetContents_18);
        textInformation->setObjectName(QString::fromUtf8("textInformation"));
        sizePolicy.setHeightForWidth(textInformation->sizePolicy().hasHeightForWidth());
        textInformation->setSizePolicy(sizePolicy);
        textInformation->setMinimumSize(QSize(384, 128));
        textInformation->setSizeIncrement(QSize(0, 0));
        textInformation->setFrameShape(QFrame::StyledPanel);
        textInformation->setFrameShadow(QFrame::Sunken);
        textInformation->setLineWidth(1);
        textInformation->setMidLineWidth(0);

        gridLayout->addWidget(textInformation, 0, 0, 1, 1);

        dwBottom->setWidget(dockWidgetContents_18);
        MainWin->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dwBottom);
        dwActiveRoutes = new QDockWidget(MainWin);
        dwActiveRoutes->setObjectName(QString::fromUtf8("dwActiveRoutes"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(dwActiveRoutes->sizePolicy().hasHeightForWidth());
        dwActiveRoutes->setSizePolicy(sizePolicy4);
        dwActiveRoutes->setMinimumSize(QSize(256, 442));
        dwActiveRoutes->setMaximumSize(QSize(300, 524287));
        dwActiveRoutes->setFloating(false);
        dwActiveRoutes->setFeatures(QDockWidget::DockWidgetMovable);
        dockWidgetContents_14 = new QWidget();
        dockWidgetContents_14->setObjectName(QString::fromUtf8("dockWidgetContents_14"));
        gridLayout_2 = new QGridLayout(dockWidgetContents_14);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(0);
        listWidget = new QListWidget(dockWidgetContents_14);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        sizePolicy.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy);
        listWidget->setDragEnabled(true);
        listWidget->setAlternatingRowColors(true);
        listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listWidget->setProperty("isWrapping", QVariant(true));
        listWidget->setViewMode(QListView::ListMode);
        listWidget->setModelColumn(0);
        listWidget->setWordWrap(true);

        gridLayout_2->addWidget(listWidget, 0, 0, 1, 1);

        dwActiveRoutes->setWidget(dockWidgetContents_14);
        MainWin->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dwActiveRoutes);
        dwSettingsControls = new QDockWidget(MainWin);
        dwSettingsControls->setObjectName(QString::fromUtf8("dwSettingsControls"));
        dwSettingsControls->setMinimumSize(QSize(183, 229));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8("icons/32x32_1000/wrench_orange.png"), QSize(), QIcon::Normal, QIcon::Off);
        dwSettingsControls->setWindowIcon(icon20);
        dwSettingsControls->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        grpbMapSource = new QGroupBox(dockWidgetContents);
        grpbMapSource->setObjectName(QString::fromUtf8("grpbMapSource"));
        formLayout = new QFormLayout(grpbMapSource);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        rbMapnik = new QRadioButton(grpbMapSource);
        btngrpMapSource = new QButtonGroup(MainWin);
        btngrpMapSource->setObjectName(QString::fromUtf8("btngrpMapSource"));
        btngrpMapSource->addButton(rbMapnik);
        rbMapnik->setObjectName(QString::fromUtf8("rbMapnik"));
        rbMapnik->setChecked(false);

        formLayout->setWidget(0, QFormLayout::LabelRole, rbMapnik);

        rbCycleMap = new QRadioButton(grpbMapSource);
        btngrpMapSource->addButton(rbCycleMap);
        rbCycleMap->setObjectName(QString::fromUtf8("rbCycleMap"));
        rbCycleMap->setChecked(true);

        formLayout->setWidget(1, QFormLayout::LabelRole, rbCycleMap);


        verticalLayout_2->addWidget(grpbMapSource);

        grpbTrainerMode = new QGroupBox(dockWidgetContents);
        grpbTrainerMode->setObjectName(QString::fromUtf8("grpbTrainerMode"));
        formLayout_2 = new QFormLayout(grpbTrainerMode);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        rbNode = new QRadioButton(grpbTrainerMode);
        btngrpTrainerMode = new QButtonGroup(MainWin);
        btngrpTrainerMode->setObjectName(QString::fromUtf8("btngrpTrainerMode"));
        btngrpTrainerMode->addButton(rbNode);
        rbNode->setObjectName(QString::fromUtf8("rbNode"));
        rbNode->setChecked(true);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, rbNode);

        rbRoute = new QRadioButton(grpbTrainerMode);
        btngrpTrainerMode->addButton(rbRoute);
        rbRoute->setObjectName(QString::fromUtf8("rbRoute"));
        rbRoute->setChecked(false);

        formLayout_2->setWidget(1, QFormLayout::LabelRole, rbRoute);


        verticalLayout_2->addWidget(grpbTrainerMode);

        dwSettingsControls->setWidget(dockWidgetContents);
        MainWin->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dwSettingsControls);

        menubar->addAction(menuRoute->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuRoute->addAction(loadFromFileAction);
        menuRoute->addAction(loadFromDatabaseAction);
        menuRoute->addSeparator();
        menuRoute->addAction(saveAction);
        menuRoute->addAction(saveAllAction);
        menuRoute->addAction(kmlExportAction);
        menuRoute->addAction(actionExport_as_tea);
        menuRoute->addSeparator();
        menuRoute->addAction(actionEdit_metadata);
        menuRoute->addSeparator();
        menuRoute->addAction(actionClose_route);
        menuRoute->addAction(actionDelete_route_from_database);
        menuRoute->addSeparator();
        menuRoute->addAction(exitAction);
        menuHelp->addAction(aboutAction);
        menuView->addAction(consoleAction);
        menuView->addSeparator();
        menuView->addAction(refreshMapAction);

        retranslateUi(MainWin);

        tabwCentre->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWin);
    } // setupUi

    void retranslateUi(QMainWindow *MainWin)
    {
        MainWin->setWindowTitle(QApplication::translate("MainWin", "Tour Evaluation and Analysis", 0, QApplication::UnicodeUTF8));
        loadFromFileAction->setText(QApplication::translate("MainWin", "Load from TEA file", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        loadFromFileAction->setStatusTip(QApplication::translate("MainWin", "Load route from TEA file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        loadFromDatabaseAction->setText(QApplication::translate("MainWin", "Load from database", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        loadFromDatabaseAction->setStatusTip(QApplication::translate("MainWin", "Load route from database", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        exitAction->setText(QApplication::translate("MainWin", "Exit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        exitAction->setStatusTip(QApplication::translate("MainWin", "Exit TEA", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        aboutAction->setText(QApplication::translate("MainWin", "About", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        aboutAction->setStatusTip(QApplication::translate("MainWin", "About the TEA program", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        consoleAction->setText(QApplication::translate("MainWin", "Console", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        consoleAction->setStatusTip(QApplication::translate("MainWin", "Display the TEA console", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        saveAction->setText(QApplication::translate("MainWin", "Save", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        saveAction->setStatusTip(QApplication::translate("MainWin", "Save currently selected route", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        saveAllAction->setText(QApplication::translate("MainWin", "Save all", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        saveAllAction->setStatusTip(QApplication::translate("MainWin", "Save all currently loaded routes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        refreshMapAction->setText(QApplication::translate("MainWin", "Refresh Map", 0, QApplication::UnicodeUTF8));
        kmlExportAction->setText(QApplication::translate("MainWin", "Export as .kml", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        kmlExportAction->setStatusTip(QApplication::translate("MainWin", "Export currently selected route as .kml", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionClose_route->setText(QApplication::translate("MainWin", "Close route", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionClose_route->setStatusTip(QApplication::translate("MainWin", "Close the selected route", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionDelete_route_from_database->setText(QApplication::translate("MainWin", "Delete route", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionDelete_route_from_database->setStatusTip(QApplication::translate("MainWin", "Delete route from database", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionEdit_metadata->setText(QApplication::translate("MainWin", "Edit metadata", 0, QApplication::UnicodeUTF8));
        actionExport_as_tea->setText(QApplication::translate("MainWin", "Export as .tea", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionExport_as_tea->setStatusTip(QApplication::translate("MainWin", "Export currently selected route as .tea", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        btnZoomIn->setText(QString());
        btnZoomOut->setText(QString());
        cbtnRotateCClockwise->setText(QString());
        cbtnRotateClockwise->setText(QString());
        tabwCentre->setTabText(tabwCentre->indexOf(tabMap), QApplication::translate("MainWin", "Map view", 0, QApplication::UnicodeUTF8));
        tabwCentre->setTabText(tabwCentre->indexOf(tabTrainer), QApplication::translate("MainWin", "Trainer view", 0, QApplication::UnicodeUTF8));
        menuRoute->setTitle(QApplication::translate("MainWin", "Route", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWin", "Help", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWin", "View", 0, QApplication::UnicodeUTF8));
        tbMain->setWindowTitle(QApplication::translate("MainWin", "Main toolbar", 0, QApplication::UnicodeUTF8));
        dwBottom->setWindowTitle(QApplication::translate("MainWin", "Console", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        textInformation->setToolTip(QApplication::translate("MainWin", "Information", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        dwActiveRoutes->setWindowTitle(QApplication::translate("MainWin", "Currently loaded routes", 0, QApplication::UnicodeUTF8));
        dwSettingsControls->setWindowTitle(QApplication::translate("MainWin", "Settings and properties", 0, QApplication::UnicodeUTF8));
        grpbMapSource->setTitle(QApplication::translate("MainWin", "Map source", 0, QApplication::UnicodeUTF8));
        rbMapnik->setText(QApplication::translate("MainWin", "Mapnik", 0, QApplication::UnicodeUTF8));
        rbCycleMap->setText(QApplication::translate("MainWin", "CycleMap", 0, QApplication::UnicodeUTF8));
        grpbTrainerMode->setTitle(QApplication::translate("MainWin", "Trainer mode", 0, QApplication::UnicodeUTF8));
        rbNode->setText(QApplication::translate("MainWin", "Node comparison", 0, QApplication::UnicodeUTF8));
        rbRoute->setText(QApplication::translate("MainWin", "Route comparison", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWin: public Ui_MainWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEA_H
