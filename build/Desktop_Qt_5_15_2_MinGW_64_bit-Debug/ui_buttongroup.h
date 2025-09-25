/********************************************************************************
** Form generated from reading UI file 'buttongroup.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUTTONGROUP_H
#define UI_BUTTONGROUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>
#include <mybutton.h>

QT_BEGIN_NAMESPACE

class Ui_ButtonGroup
{
public:
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *startPage;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    MyButton *start;
    QSpacerItem *horizontalSpacer_2;
    QWidget *playCardPage;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    MyButton *playCard;
    QSpacerItem *horizontalSpacer_4;
    QWidget *playOrPassPage;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_5;
    MyButton *pass;
    QSpacerItem *horizontalSpacer_6;
    MyButton *playCard1;
    QSpacerItem *horizontalSpacer_7;
    QWidget *callLordPage;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_8;
    MyButton *giveup;
    QSpacerItem *horizontalSpacer_9;
    MyButton *oneScore;
    QSpacerItem *horizontalSpacer_10;
    MyButton *twoScore;
    QSpacerItem *horizontalSpacer_11;
    MyButton *threeScore;
    QSpacerItem *horizontalSpacer_12;
    QWidget *page;

    void setupUi(QWidget *ButtonGroup)
    {
        if (ButtonGroup->objectName().isEmpty())
            ButtonGroup->setObjectName(QString::fromUtf8("ButtonGroup"));
        ButtonGroup->resize(509, 182);
        horizontalLayout = new QHBoxLayout(ButtonGroup);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(ButtonGroup);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        startPage = new QWidget();
        startPage->setObjectName(QString::fromUtf8("startPage"));
        horizontalLayout_2 = new QHBoxLayout(startPage);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(187, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        start = new MyButton(startPage);
        start->setObjectName(QString::fromUtf8("start"));

        horizontalLayout_2->addWidget(start);

        horizontalSpacer_2 = new QSpacerItem(187, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        stackedWidget->addWidget(startPage);
        playCardPage = new QWidget();
        playCardPage->setObjectName(QString::fromUtf8("playCardPage"));
        horizontalLayout_3 = new QHBoxLayout(playCardPage);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(187, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        playCard = new MyButton(playCardPage);
        playCard->setObjectName(QString::fromUtf8("playCard"));

        horizontalLayout_3->addWidget(playCard);

        horizontalSpacer_4 = new QSpacerItem(187, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        stackedWidget->addWidget(playCardPage);
        playOrPassPage = new QWidget();
        playOrPassPage->setObjectName(QString::fromUtf8("playOrPassPage"));
        horizontalLayout_4 = new QHBoxLayout(playOrPassPage);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_5 = new QSpacerItem(127, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        pass = new MyButton(playOrPassPage);
        pass->setObjectName(QString::fromUtf8("pass"));

        horizontalLayout_4->addWidget(pass);

        horizontalSpacer_6 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        playCard1 = new MyButton(playOrPassPage);
        playCard1->setObjectName(QString::fromUtf8("playCard1"));

        horizontalLayout_4->addWidget(playCard1);

        horizontalSpacer_7 = new QSpacerItem(127, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        stackedWidget->addWidget(playOrPassPage);
        callLordPage = new QWidget();
        callLordPage->setObjectName(QString::fromUtf8("callLordPage"));
        horizontalLayout_5 = new QHBoxLayout(callLordPage);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_8 = new QSpacerItem(7, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);

        giveup = new MyButton(callLordPage);
        giveup->setObjectName(QString::fromUtf8("giveup"));

        horizontalLayout_5->addWidget(giveup);

        horizontalSpacer_9 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_9);

        oneScore = new MyButton(callLordPage);
        oneScore->setObjectName(QString::fromUtf8("oneScore"));

        horizontalLayout_5->addWidget(oneScore);

        horizontalSpacer_10 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_10);

        twoScore = new MyButton(callLordPage);
        twoScore->setObjectName(QString::fromUtf8("twoScore"));

        horizontalLayout_5->addWidget(twoScore);

        horizontalSpacer_11 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_11);

        threeScore = new MyButton(callLordPage);
        threeScore->setObjectName(QString::fromUtf8("threeScore"));

        horizontalLayout_5->addWidget(threeScore);

        horizontalSpacer_12 = new QSpacerItem(7, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_12);

        stackedWidget->addWidget(callLordPage);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        stackedWidget->addWidget(page);

        horizontalLayout->addWidget(stackedWidget);


        retranslateUi(ButtonGroup);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(ButtonGroup);
    } // setupUi

    void retranslateUi(QWidget *ButtonGroup)
    {
        ButtonGroup->setWindowTitle(QCoreApplication::translate("ButtonGroup", "Form", nullptr));
        start->setText(QString());
        playCard->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        pass->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        playCard1->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        giveup->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        oneScore->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        twoScore->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        threeScore->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ButtonGroup: public Ui_ButtonGroup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUTTONGROUP_H
