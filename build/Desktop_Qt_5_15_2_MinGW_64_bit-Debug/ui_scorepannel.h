/********************************************************************************
** Form generated from reading UI file 'scorepannel.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCOREPANNEL_H
#define UI_SCOREPANNEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScorePannel
{
public:
    QGridLayout *gridLayout;
    QLabel *m_user;
    QLabel *mScore;
    QLabel *score1;
    QLabel *robotLeft;
    QLabel *leftScore;
    QLabel *score2;
    QLabel *robotRight;
    QLabel *rightScore;
    QLabel *score3;

    void setupUi(QWidget *ScorePannel)
    {
        if (ScorePannel->objectName().isEmpty())
            ScorePannel->setObjectName(QString::fromUtf8("ScorePannel"));
        ScorePannel->resize(172, 147);
        gridLayout = new QGridLayout(ScorePannel);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_user = new QLabel(ScorePannel);
        m_user->setObjectName(QString::fromUtf8("m_user"));
        QFont font;
        font.setFamily(QString::fromUtf8("Mistral"));
        font.setPointSize(12);
        font.setBold(false);
        m_user->setFont(font);
        m_user->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        m_user->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(m_user, 0, 0, 1, 1);

        mScore = new QLabel(ScorePannel);
        mScore->setObjectName(QString::fromUtf8("mScore"));
        mScore->setFont(font);
        mScore->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        mScore->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(mScore, 0, 1, 1, 1);

        score1 = new QLabel(ScorePannel);
        score1->setObjectName(QString::fromUtf8("score1"));
        score1->setFont(font);
        score1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        score1->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(score1, 0, 2, 1, 1);

        robotLeft = new QLabel(ScorePannel);
        robotLeft->setObjectName(QString::fromUtf8("robotLeft"));
        robotLeft->setFont(font);
        robotLeft->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        robotLeft->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(robotLeft, 1, 0, 1, 1);

        leftScore = new QLabel(ScorePannel);
        leftScore->setObjectName(QString::fromUtf8("leftScore"));
        leftScore->setFont(font);
        leftScore->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        leftScore->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(leftScore, 1, 1, 1, 1);

        score2 = new QLabel(ScorePannel);
        score2->setObjectName(QString::fromUtf8("score2"));
        score2->setFont(font);
        score2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        score2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(score2, 1, 2, 1, 1);

        robotRight = new QLabel(ScorePannel);
        robotRight->setObjectName(QString::fromUtf8("robotRight"));
        robotRight->setFont(font);
        robotRight->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        robotRight->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(robotRight, 2, 0, 1, 1);

        rightScore = new QLabel(ScorePannel);
        rightScore->setObjectName(QString::fromUtf8("rightScore"));
        rightScore->setFont(font);
        rightScore->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        rightScore->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(rightScore, 2, 1, 1, 1);

        score3 = new QLabel(ScorePannel);
        score3->setObjectName(QString::fromUtf8("score3"));
        score3->setFont(font);
        score3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        score3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(score3, 2, 2, 1, 1);


        retranslateUi(ScorePannel);

        QMetaObject::connectSlotsByName(ScorePannel);
    } // setupUi

    void retranslateUi(QWidget *ScorePannel)
    {
        ScorePannel->setWindowTitle(QCoreApplication::translate("ScorePannel", "Form", nullptr));
        m_user->setText(QCoreApplication::translate("ScorePannel", "\346\210\221:", nullptr));
        mScore->setText(QString());
        score1->setText(QCoreApplication::translate("ScorePannel", "\345\210\206", nullptr));
        robotLeft->setText(QCoreApplication::translate("ScorePannel", "\345\267\246\344\276\247\346\234\272\345\231\250\344\272\272:", nullptr));
        leftScore->setText(QString());
        score2->setText(QCoreApplication::translate("ScorePannel", "\345\210\206", nullptr));
        robotRight->setText(QCoreApplication::translate("ScorePannel", "\345\217\263\344\276\247\346\234\272\345\231\250\344\272\272:", nullptr));
        rightScore->setText(QString());
        score3->setText(QCoreApplication::translate("ScorePannel", "\345\210\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScorePannel: public Ui_ScorePannel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCOREPANNEL_H
