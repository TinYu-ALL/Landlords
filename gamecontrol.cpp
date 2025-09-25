#include "gamecontrol.h"
#include "playhand.h"
#include <QRandomGenerator>
#include <QTimer>

GameControl::GameControl(QObject *parent)
    : QObject{parent}
{}

void GameControl::playerInit()
{
    m_robotLeft = new Robot("机器人A",this);
    m_robotRight = new Robot("机器人B",this);
    m_user = new UserPlayer("帅小伙",this);

    //头像显示方向
    m_robotLeft->setDirection(Player::Left);
    m_robotRight->setDirection(Player::Right);
    m_user->setDirection(Player::Right);

    //性别
    Player::Sex sex ;
    sex= (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotLeft->setSex(sex);
    sex= (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotRight->setSex(sex);
    sex= (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_user->setSex(sex);

    //上下家
    //user
    m_user->setPrevPLayer(m_robotLeft);
    m_user->setNextPlayer(m_robotRight);
    //m_robotLeft
    m_robotLeft->setPrevPLayer(m_robotRight);
    m_robotLeft->setNextPlayer(m_user);
    //m_robotRight
    m_robotRight->setPrevPLayer(m_user);
    m_robotRight->setNextPlayer(m_robotLeft);

    //指定当前玩家
    m_currPlayer=m_user;

    //处理玩家发射出的信号
    connect(m_user,&Player::noticyGrabLordBet,this,&GameControl::onGrabLordBet);
    connect(m_robotLeft,&Player::noticyGrabLordBet,this,&GameControl::onGrabLordBet);
    connect(m_robotRight,&Player::noticyGrabLordBet,this,&GameControl::onGrabLordBet);

    //传递出牌玩家和打出的牌
    connect(this,&GameControl::controlNoticyPendInfo,m_robotLeft,&Robot::setPendingInfo);
    connect(this,&GameControl::controlNoticyPendInfo,m_robotRight,&Robot::setPendingInfo);
    connect(this,&GameControl::controlNoticyPendInfo,m_user,&UserPlayer::setPendingInfo);

    //处理出牌
    connect(m_robotLeft,&Robot::noticyPlayHand,this,&GameControl::onPlayHand);
    connect(m_robotRight,&Robot::noticyPlayHand,this,&GameControl::onPlayHand);
    connect(m_user,&UserPlayer::noticyPlayHand,this,&GameControl::onPlayHand);
}

Robot *GameControl::getLeftRobot()
{
    return m_robotLeft;
}

Robot *GameControl::getRightRobot()
{
    return m_robotRight;
}

UserPlayer *GameControl::getUserPlayer()
{
    return m_user;
}

void GameControl::setCurrentPlayer(Player *player)
{
    m_currPlayer = player;
}

Player *GameControl::getCurrentPlyer()
{
    return m_currPlayer;
}

Player *GameControl::getPendPlayer()
{
    return m_pendPlayer;
}

Cards GameControl::getPendCards()
{
    return m_pendCards;
}

void GameControl::initAllCards()
{
    m_allCards.clear();
    for(auto p = Card::Card_Begin+1;p<Card::Card_SJ;p++)
    {
        for(auto s = Card::Suit_Begin+1;s<Card::Suit_End;s++)
        {
            Card c((Card::CardPoint)p,(Card::CardSuit)s);
            m_allCards.add(c);
        }
    }
    m_allCards.add(Card(Card::Card_SJ,Card::Suit_Begin));
    m_allCards.add(Card(Card::Card_BJ,Card::Suit_Begin));
}

Card GameControl::takeOneCard()
{
    return m_allCards.takeRandomCard();
}

void GameControl::resetCardData()
{
    //洗牌
    initAllCards();
    //清空所有玩家的牌
    m_robotLeft->clearCards();
    m_robotRight->clearCards();
    m_user->clearCards();
    //重置发牌的玩家和牌
    m_pendPlayer = nullptr;
    m_pendCards.clear();
}

void GameControl::startLordCall()
{
    m_currPlayer->prepareCallLord();
    emit playerStatusChanged(m_currPlayer,ThinkingForCallLord);
}

void GameControl::becomeLord(Player *player,int bet)
{
    m_bet = bet;
    player->setRole(Player::Lord);
    player->getPrevPlayer()->setRole(Player::Farmer);
    player->getNextPlayer()->setRole(Player::Farmer);

    m_currPlayer=player;
    m_currPlayer->storeDispatchCard(m_allCards);

    QTimer::singleShot(1000,player,[=](){
        emit playerStatusChanged(player,ThinkingForPlayHand);
        emit controlGameStatusChange(PlayingHand);
        m_currPlayer->preparePlayHand();
    });
}

void GameControl::clearPlayerScore()
{
    m_robotLeft->setScore(0);
    m_robotRight->setScore(0);
    m_user->setScore(0);
}

void GameControl::onGrabLordBet(Player *player, int point)
{
    //通知主界面玩家叫地主了
    if(point == 0 || m_pointCompare.maxPoint>=point)
    {
        //放弃抢地主
        emit this->controlNoticyGrabLord(player,0,false);
    }else if(m_pointCompare.maxPoint == 0 && point>0)
    {
        //第一个抢地主的
        emit this->controlNoticyGrabLord(player,point,true);
    }else
    {
        //第2、3个抢地主的
        emit this->controlNoticyGrabLord(player,point,false);
    }
    //判断分数是否3分，是成为地主
    if(point == 3)
    {
        //玩家成为地主
        becomeLord(player,point);
        //清空数据
        m_pointCompare.reset();
        return;
    }
    //下注不够三分进行比较，最高的为地主
    if(m_pointCompare.maxPoint < point)
    {
        m_pointCompare.maxPoint = point;
        m_pointCompare.player = player;
    }
    m_pointCompare.num++;
    if(m_pointCompare.num == 3)
    {
        if(m_pointCompare.maxPoint == 0)
        {
            //重新发牌
            emit this->controlGameStatusChange(DispatchCard);
        }else
        {
            becomeLord(m_pointCompare.player,m_pointCompare.maxPoint);
        }
        m_pointCompare.reset();
        return;
    }
    //切换玩家，进行下一次叫地主
    m_currPlayer = player->getNextPlayer();
    //发信号给主界面，告知当前状态为抢地主
    emit this->playerStatusChanged(m_currPlayer,PlayerStatus::ThinkingForCallLord);
    m_currPlayer->prepareCallLord();
}

void GameControl::onPlayHand(Player *player,const Cards &cards)
{
    //1.玩家出牌信号通知主界面
    emit this->controlNoticyPlayHand(player,cards);
    //2.如果不是空牌，将出牌玩家和牌通过信号转发给其他玩家
    if(!cards.isEmpty())
    {
        m_pendCards = cards;
        m_pendPlayer = player;
        emit this->controlNoticyPendInfo(m_pendPlayer,m_pendCards);
    }
    //如果有炸弹，底分翻倍
    Cards myCards = const_cast<Cards&>(cards);
    PlayHand::HandType type = PlayHand(myCards).getType();
    if(type == PlayHand::Hand_Bomb || type == PlayHand::Hand_Bomb_Jokers)
    {
        m_bet *= 2;
    }
    //3.如果出完牌了，计算得分
    if(player->getCards().isEmpty())
    {
        Player* prv = player->getPrevPlayer();
        Player* next = player->getNextPlayer();
        if(player->getRole() == Player::Lord)
        {
            player->setScore(player->getSocre() + 2 * m_bet);
            player->setWin(true);
            prv->setScore(prv->getSocre() - m_bet);
            prv->setWin(false);
            next->setScore(next->getSocre() - m_bet);
            next->setWin(false);
        }else
        {
            player->setScore(player->getSocre() + m_bet);
            player->setWin(true);
            if(prv->getRole() == Player::Lord)
            {
                prv->setScore(prv->getSocre() - 2 * m_bet);
                prv->setWin(false);
                next->setScore(next->getSocre() + m_bet);
                next->setWin(true);
            }else
            {
                prv->setScore(prv->getSocre() + m_bet);
                prv->setWin(true);
                next->setScore(next->getSocre() - 2 * m_bet);
                next->setWin(false);
            }
        }
        emit playerStatusChanged(player,PlayerStatus::Winning);
        return;
    }
    //4.如果没有出完或是空牌，下一个玩家出牌
    m_currPlayer = player->getNextPlayer();
    m_currPlayer->preparePlayHand();
    emit playerStatusChanged(m_currPlayer,PlayerStatus::ThinkingForPlayHand);
}

int GameControl::getMaxBet()
{
    return m_pointCompare.maxPoint;
}
