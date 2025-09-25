#include "card.h"

Card::Card() {}

Card::Card(CardPoint point, CardSuit suit)
{
    setPoint(point);
    setSuit(suit);
}

void Card::setPoint(CardPoint point)
{
    m_point =  point;
}

void Card::setSuit(CardSuit suit)
{
    m_suit = suit;
}

Card::CardPoint Card::Point() const
{
    return m_point;
}

Card::CardSuit Card::Suit() const
{
    return m_suit;
}

bool lessSort(const Card &c1, const Card &c2)
{
    if(c1.Point()==c2.Point())
    {
        return c1.Suit()<c2.Suit();
    }else
    {
        return c1.Point()<c2.Point();
    }
}

bool greaterSort(const Card &c1, const Card &c2)
{
    if(c1.Point()==c2.Point())
    {
        return c1.Suit()>c2.Suit();
    }else
    {
        return c1.Point()>c2.Point();
    }
}


bool operator ==(const Card &left, const Card &right)
{
    return (left.Point()==right.Point()&&left.Suit()==right.Suit());
}


uint qHash(const Card &card)
{
    return card.Point()*100+card.Suit();
}


bool operator <(const Card &left, const Card &right)
{
    if(left.Point()!=right.Point())
    {
        return left.Point()<right.Point();
    }
    return left.Suit()<right.Suit();
}

