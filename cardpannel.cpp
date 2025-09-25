#include "cardpannel.h"

#include <QPainter>

CardPannel::CardPannel(QWidget *parent)
    : QWidget{parent}
{}

void CardPannel::setPixmap(const QPixmap &fornt,const QPixmap &black)
{
    m_fornt = fornt;
    m_black = black;

    setFixedSize(m_fornt.size());

    update();
}

QPixmap CardPannel::getPixmap()
{
    return m_fornt;
}

void CardPannel::setCardSide(bool flag)
{
    m_isfront = flag;
}

bool CardPannel::isForntSide()
{
    return m_isfront;
}

void CardPannel::setSelected(bool flag)
{
    m_isSelect = flag;
}

bool CardPannel::isSelected()
{
    return m_isSelect;
}

void CardPannel::setCard(const Card &card)
{
    m_card = card;
}

Card CardPannel::getCard()
{
    return m_card;
}

void CardPannel::cardOwner(Player *player)
{
    m_player = player;
}

Player* CardPannel::getOwner()
{
    return m_player;
}

void CardPannel::click()
{
    emit currSelectCard(Qt::LeftButton);
}

void CardPannel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    if(m_isfront)
    {
        p.drawPixmap(rect(),m_fornt);
    }else
    {
        p.drawPixmap(rect(),m_black);
    }
}

void CardPannel::mousePressEvent(QMouseEvent *event)
{
    emit currSelectCard(event->button());
}
