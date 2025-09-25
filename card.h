#ifndef CARD_H
#define CARD_H

#include <QVector>


class Card
{
public:
    enum CardPoint{
        Card_Begin,
        Card_3,
        Card_4,
        Card_5,
        Card_6,
        Card_7,
        Card_8,
        Card_9,
        Card_10,
        Card_J,
        Card_Q,
        Card_K,
        Card_A,
        Card_2,
        Card_SJ,
        Card_BJ,
        Card_End
    };
    enum CardSuit{
        Suit_Begin,
        Diamond,    //♦
        Club,       //♣
        Heart,      //红心
        Spade,      //♠
        Suit_End
    };

    Card();
    Card(CardPoint point,CardSuit suit);

    void setPoint(CardPoint point);
    void setSuit(CardSuit suit);
    CardPoint Point() const;
    CardSuit Suit() const;

private:
    CardSuit m_suit;
    CardPoint m_point;
};
//定义别名
using CardList =QVector<Card>;

//比较
bool lessSort(const Card& c1,const Card& c2);
bool greaterSort(const Card& c1,const Card& c2);

//重载==
bool operator == (const Card& left,const Card& right);

//重载<
bool operator < (const Card& left,const Card& right);

//重写全局函数
uint qHash(const Card& card);

#endif // CARD_H
