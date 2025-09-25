/********************************************************************************
** Form generated from reading UI file 'gamepannel.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEPANNEL_H
#define UI_GAMEPANNEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "buttongroup.h"
#include "scorepannel.h"

QT_BEGIN_NAMESPACE

class Ui_GamePanel
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    ScorePannel *scorePannel;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    ButtonGroup *btnGroup;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QMainWindow *GamePanel)
    {
        if (GamePanel->objectName().isEmpty())
            GamePanel->setObjectName(QString::fromUtf8("GamePanel"));
        GamePanel->resize(800, 600);
        centralwidget = new QWidget(GamePanel);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        scorePannel = new ScorePannel(widget);
        scorePannel->setObjectName(QString::fromUtf8("scorePannel"));

        horizontalLayout->addWidget(scorePannel);

        horizontalSpacer = new QSpacerItem(736, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(widget);

        verticalSpacer = new QSpacerItem(20, 474, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        btnGroup = new ButtonGroup(centralwidget);
        btnGroup->setObjectName(QString::fromUtf8("btnGroup"));

        verticalLayout->addWidget(btnGroup);

        verticalSpacer_2 = new QSpacerItem(20, 120, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        GamePanel->setCentralWidget(centralwidget);

        retranslateUi(GamePanel);

        QMetaObject::connectSlotsByName(GamePanel);
    } // setupUi

    void retranslateUi(QMainWindow *GamePanel)
    {
        GamePanel->setWindowTitle(QCoreApplication::translate("GamePanel", "GamePanel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GamePanel: public Ui_GamePanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEPANNEL_H
