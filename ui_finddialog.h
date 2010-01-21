/********************************************************************************
** Form generated from reading UI file 'finddialog.ui'
**
** Created: Thu 14. Jan 21:52:22 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDDIALOG_H
#define UI_FINDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FindDialogClass
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *boxQuery;
    QFormLayout *formLayout;
    QLabel *lblType;
    QLineEdit *edtValue;
    QLabel *lblValue;
    QHBoxLayout *horizontalLayout;
    QComboBox *cboxType;
    QComboBox *cboxOperator;
    QDialogButtonBox *bboxQuery;
    QListWidget *lwResult;
    QDialogButtonBox *bboxChoice;

    void setupUi(QDialog *FindDialogClass)
    {
        if (FindDialogClass->objectName().isEmpty())
            FindDialogClass->setObjectName(QString::fromUtf8("FindDialogClass"));
        FindDialogClass->resize(431, 398);
        QIcon icon;
        icon.addFile(QString::fromUtf8("icons/32x32_0320/cup_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        FindDialogClass->setWindowIcon(icon);
        FindDialogClass->setSizeGripEnabled(false);
        verticalLayout = new QVBoxLayout(FindDialogClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        boxQuery = new QGroupBox(FindDialogClass);
        boxQuery->setObjectName(QString::fromUtf8("boxQuery"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(boxQuery->sizePolicy().hasHeightForWidth());
        boxQuery->setSizePolicy(sizePolicy);
        formLayout = new QFormLayout(boxQuery);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        lblType = new QLabel(boxQuery);
        lblType->setObjectName(QString::fromUtf8("lblType"));

        formLayout->setWidget(0, QFormLayout::LabelRole, lblType);

        edtValue = new QLineEdit(boxQuery);
        edtValue->setObjectName(QString::fromUtf8("edtValue"));

        formLayout->setWidget(3, QFormLayout::FieldRole, edtValue);

        lblValue = new QLabel(boxQuery);
        lblValue->setObjectName(QString::fromUtf8("lblValue"));

        formLayout->setWidget(3, QFormLayout::LabelRole, lblValue);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cboxType = new QComboBox(boxQuery);
        cboxType->setObjectName(QString::fromUtf8("cboxType"));

        horizontalLayout->addWidget(cboxType);

        cboxOperator = new QComboBox(boxQuery);
        cboxOperator->setObjectName(QString::fromUtf8("cboxOperator"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cboxOperator->sizePolicy().hasHeightForWidth());
        cboxOperator->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(cboxOperator);


        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout);

        bboxQuery = new QDialogButtonBox(boxQuery);
        bboxQuery->setObjectName(QString::fromUtf8("bboxQuery"));
        bboxQuery->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        formLayout->setWidget(4, QFormLayout::FieldRole, bboxQuery);


        verticalLayout->addWidget(boxQuery);

        lwResult = new QListWidget(FindDialogClass);
        lwResult->setObjectName(QString::fromUtf8("lwResult"));
        lwResult->setAlternatingRowColors(true);
        lwResult->setSelectionMode(QAbstractItemView::MultiSelection);

        verticalLayout->addWidget(lwResult);

        bboxChoice = new QDialogButtonBox(FindDialogClass);
        bboxChoice->setObjectName(QString::fromUtf8("bboxChoice"));
        bboxChoice->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Open);

        verticalLayout->addWidget(bboxChoice);


        retranslateUi(FindDialogClass);

        QMetaObject::connectSlotsByName(FindDialogClass);
    } // setupUi

    void retranslateUi(QDialog *FindDialogClass)
    {
        FindDialogClass->setWindowTitle(QApplication::translate("FindDialogClass", "Load route from database", 0, QApplication::UnicodeUTF8));
        boxQuery->setTitle(QApplication::translate("FindDialogClass", "Query", 0, QApplication::UnicodeUTF8));
        lblType->setText(QApplication::translate("FindDialogClass", "Type", 0, QApplication::UnicodeUTF8));
        lblValue->setText(QApplication::translate("FindDialogClass", "Value", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FindDialogClass: public Ui_FindDialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDDIALOG_H
