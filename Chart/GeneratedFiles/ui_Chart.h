/********************************************************************************
** Form generated from reading UI file 'Chart.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHART_H
#define UI_CHART_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChartClass
{
public:

    void setupUi(QWidget *ChartClass)
    {
        if (ChartClass->objectName().isEmpty())
            ChartClass->setObjectName(QStringLiteral("ChartClass"));
        ChartClass->resize(600, 400);

        retranslateUi(ChartClass);

        QMetaObject::connectSlotsByName(ChartClass);
    } // setupUi

    void retranslateUi(QWidget *ChartClass)
    {
        ChartClass->setWindowTitle(QApplication::translate("ChartClass", "Chart", 0));
    } // retranslateUi

};

namespace Ui {
    class ChartClass: public Ui_ChartClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHART_H
