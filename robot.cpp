#include "robot.h"

#include "robotgraplord.h"
#include "robotplayhand.h"
#include "strategy.h"

#include <QDebug>
#include <QTimer>

Robot::Robot(QObject *parent)
    : Player{parent}
{
    m_type = Player::Robot;
}

void Robot::prepareCallLord()
{
    RobotGraplord* robotThread = new RobotGraplord(this);
    QThread* thread = new QThread(this);

    robotThread->moveToThread(thread);

    // 线程启动后调用 robotThread 的 work 方法
    connect(thread, &QThread::started, robotThread, &RobotGraplord::work);

    // 任务完成后清理线程
    connect(robotThread, &RobotGraplord::finish, thread, &QThread::quit);
    connect(robotThread, &RobotGraplord::finish, robotThread, &QThread::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    QTimer::singleShot(1000,this,[=](){
        thread->start();
    });

    // RobotGraplord* robotThread = new RobotGraplord(this);
    // connect(robotThread,&RobotGraplord::finish,this,[=](){
    //     robotThread->deleteLater();
    // });
    // robotThread->start();
}

void Robot::preparePlayHand()
{
    RobotPlayHand* robotThread2 = new RobotPlayHand(this);
    QThread* thread2 = new QThread(this);

    robotThread2->moveToThread(thread2);

    // 线程启动后调用 robotThread2 的 work 方法
    connect(thread2, &QThread::started, robotThread2, &RobotPlayHand::work);

    // 策略执行完后回主线程执行出牌
    connect(robotThread2, &RobotPlayHand::strategyReady, this,[=](const Cards& cards){
        // qDebug() << "收到 AI 选牌信号，牌数：";
        playHand(cards);  // 主线程中执行，安全
        thread2->quit();
    });

    // 任务完成后清理线程
    connect(thread2, &QThread::finished, robotThread2, &QObject::deleteLater);
    connect(thread2, &QThread::finished, thread2, &QObject::deleteLater);

    QTimer::singleShot(1000,this,[=](){
        thread2->start();
    });

    // RobotPlayHand* robotThread2 = new RobotPlayHand(this);
    // connect(robotThread2,&RobotPlayHand::finish,this,[=](){
    //     robotThread2->deleteLater();
    // });
    // robotThread2->start();
}

void Robot::thinkCallLord()
{
    /*
     基于手里的牌计算权重
    大小王：6
    顺子/炸弹：5
    三张：4
    2：3
    对子：1
    */

    int weight = 0;
    Strategy st(this,m_cards);
    weight += st.findRangeCards(Card::Card_SJ,Card::Card_BJ).cardCount() * 6;

    QVector<Cards> seq = st.pickSeqSignels(); //小于Q
    weight += seq.size() *5;

    QVector<Cards> bombs = st.findCountCards(4);
    weight += bombs.size() *5;

    weight += m_cards.pointCount(Card::Card_2) *3;

    Cards temp = m_cards;
    temp.remove(seq);
    temp.remove(bombs);
    Cards card2 = Strategy(this,temp).findRangeCards(Card::Card_2,Card::Card_2);
    temp.remove(card2);
    QVector<Cards> three = Strategy(this,temp).findCountCards(3);
    weight += three.size() *4;

    temp.remove(three);
    QVector<Cards> pair = Strategy(this,temp).findCountCards(2);
    weight += pair.size() *1;

    if(weight >= 22)
    {
        grabLordBet(3);
    }else if(weight >= 18)
    {
        grabLordBet(2);
    }else if(weight >= 10)
    {
        grabLordBet(1);
    }else grabLordBet(0);
}

void Robot::thinkPlayHand()
{
    Strategy st(this, m_cards);
    // qDebug() << "AI准备出牌，当前手牌：" << m_cards.cardCount();
    Cards playCards = st.makeStrategy();
    // qDebug() << "AI选牌：" << playCards.cardCount();

    // //添加调试日志
    // if (playCards.isEmpty()) {
    //     qDebug() << "AI没有选牌，检查策略问题";
    // } else {
    //     qDebug() << "AI选出的牌数：" << playCards.cardCount();
    // }
    // qDebug() << "thinkPlayHand thread:" << QThread::currentThread();
    // qDebug() << "Robot object thread:" << this->thread();

    playHand(playCards);
}

