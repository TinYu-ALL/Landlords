#include "player.h"

Player::Player(QObject *parent)
    : QObject{parent}
{}

Player::Player(QString name, QObject *parent)
    :QObject{parent},
    m_name{name}
{
    m_score = 0;
    m_isWin = false;
    m_pendPlayer = nullptr;
}

void Player::setName(QString name)
{
    m_name = name;
}

QString Player::getName()
{
    return m_name;
}

void Player::setRole(Role role)
{
    m_role = role;
}

Player::Role Player::getRole()
{
    return m_role;
}

void Player::setSex(Sex sex)
{
    m_sex = sex;
}

Player::Sex Player::getSex()
{
    return m_sex;
}

void Player::setDirection(Direction direction)
{
    m_direction = direction;
}

Player::Direction Player::getDirection()
{
    return m_direction;
}

void Player::setType(Type type)
{
    m_type = type;
}

Player::Type Player::getType()
{
    return m_type;
}

void Player::setScore(int score)
{
    m_score = score;
}

int Player::getSocre()
{
   return m_score;
}

void Player::setWin(bool flag)
{
    m_isWin = flag;
}

bool Player::isWin()
{
    return m_isWin;
}

void Player::setPrevPLayer(Player *prev)
{
    m_prev = prev;
}

void Player::setNextPlayer(Player *next)
{
    m_next = next;
}

Player *Player::getPrevPlayer()
{
    return m_prev;
}

Player *Player::getNextPlayer()
{
    return m_next;
}

void Player::grabLordBet(int point)
{
    //已经抢地主信号
    emit noticyGrabLordBet(this,point);
}

void Player::storeDispatchCard(const Card &card)
{
    m_cards.add(card);
    Cards c;
    c.add(card);
    emit this->noticyStoreCard(this,c);
}

void Player::storeDispatchCard(const Cards &cards)
{
    m_cards.add(cards);
    emit this->noticyStoreCard(this,cards);
}

void Player::playHand(const Cards &cards)
{
    m_cards.remove(cards);
    emit this->noticyPlayHand(this,cards);
}

Cards Player::getCards()
{
    return m_cards;
}

void Player::clearCards()
{
    m_cards.clear();
}

void Player::setPendingInfo(Player *player,const Cards &cards)
{
    m_pendPlayer = player;
    m_pendCards = cards;
}

Player *Player::getPendPlayer()
{
    return m_pendPlayer;
}

Cards Player::getPendCards()
{
    return m_pendCards;
}

void Player::prepareCallLord()
{

}

void Player::preparePlayHand()
{

}

void Player::thinkCallLord()
{

}

void Player::thinkPlayHand()
{

}






