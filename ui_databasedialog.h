/********************************************************************************
** Form generated from reading ui file 'databasedialog.ui'
**
** Created: Wed 30. Dec 14:27:42 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DATABASEDIALOG_H
#define UI_DATABASEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>

QT_BEGIN_NAMESPACE

class Ui_DatabaseDialogClass
{
public:
    QListView *listView;
    QDialogButtonBox *buttonBox;
    QDialogButtonBox *buttonBox_2;

    void setupUi(QDialog *DatabaseDialogClass)
    {
        if (DatabaseDialogClass->objectName().isEmpty())
            DatabaseDialogClass->setObjectName(QString::fromUtf8("DatabaseDialogClass"));
        DatabaseDialogClass->resize(494, 449);
        listView = new QListView(DatabaseDialogClass);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(10, 10, 471, 391));
        buttonBox = new QDialogButtonBox(DatabaseDialogClass);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(290, 420, 441, 23));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox_2 = new QDialogButtonBox(DatabaseDialogClass);
        buttonBox_2->setObjectName(QString::fromUtf8("buttonBox_2"));
        buttonBox_2->setGeometry(QRect(320, 410, 156, 23));
        buttonBox_2->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Open);

        retranslateUi(DatabaseDialogClass);

        QMetaObject::connectSlotsByName(DatabaseDialogClass);
    } // setupUi

    void retranslateUi(QDialog *DatabaseDialogClass)
    {
        DatabaseDialogClass->setWindowTitle(QApplication::translate("DatabaseDialogClass", "DatabaseDialog", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DatabaseDialogClass);
    } // retranslateUi

};

namespace Ui {
    class DatabaseDialogClass: public Ui_DatabaseDialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATABASEDIALOG_H
