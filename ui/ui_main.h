/********************************************************************************
** Form generated from reading UI file 'main.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QWidget>
#include <SearchLineEdit.h>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    SearchLineEdit *searchLineEdit;
    QLabel *promptLabel;
    QListView *listView;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(764, 421);
        searchLineEdit = new SearchLineEdit(Widget);
        searchLineEdit->setObjectName(QStringLiteral("searchLineEdit"));
        searchLineEdit->setGeometry(QRect(230, 70, 381, 30));
        promptLabel = new QLabel(Widget);
        promptLabel->setObjectName(QStringLiteral("promptLabel"));
        promptLabel->setGeometry(QRect(140, 70, 91, 22));
        promptLabel->setAlignment(Qt::AlignCenter);
        listView = new QListView(Widget);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setGeometry(QRect(130, 110, 491, 281));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        searchLineEdit->setText(QApplication::translate("Widget", "git hub ui ", nullptr));
        promptLabel->setText(QApplication::translate("Widget", "Find in d:/", nullptr));
        Q_UNUSED(Widget);
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_H
