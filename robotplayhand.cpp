#include "robotplayhand.h"

#include "strategy.h"

#include <QTimer>
#include <QDebug>

RobotPlayHand::RobotPlayHand(Player *robot, QObject *parent)
    : QObject{parent},m_cards(robot->getCards())
{
    m_player = robot;
}

void RobotPlayHand::work()
{
    Strategy st(m_player,m_cards);
    // qDebug() << "AI准备出牌，当前手牌：" << m_cards.cardCount();
    Cards playCards = st.makeStrategy();
    // qDebug() << "AI选牌：" << playCards.cardCount();
    // qDebug() << "RobotPlayHand::work playCards size=" << playCards.cardCount();

    // // 添加调试日志
    // if (playCards.isEmpty()) {
    //     qDebug() << "AI没有选牌，检查策略问题";
    // } else {
    //     qDebug() << "AI选出的牌数：" << playCards.cardCount();
    // }
    Cards& cards = const_cast<Cards&>(playCards);

    QTimer::singleShot(2000,[=](){
        emit strategyReady(cards);
    });
}

// void RobotPlayHand::run()
// {
//     msleep(1000);
//     m_player->thinkPlayHand();
// }


