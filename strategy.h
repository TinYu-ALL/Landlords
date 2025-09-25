#ifndef STRATEGY_H
#define STRATEGY_H

#include "Player.h"
#include "playhand.h"


class Strategy
{
public:
    Strategy(Player* player,const Cards& cards);

    //找出指定数量（count）的相同点数（point）的牌   3\33\333\3333
    Cards findSamePointCards(Card::CardPoint point,int count);
    //找出所有点数数量为count的牌（顺子,对子，飞机……）123456\1122334455\111222…… ==》得到一个多张扑克牌数组
    QVector<Cards> findCountCards(int count);
    //根据点数范围找牌
    Cards findRangeCards(Card::CardPoint begin,Card::CardPoint end);
    //按牌型找牌，还要比指定的牌大
    QVector<Cards> findTypeCards(PlayHand hand,bool flag);

    /******************************************************************/
    //出牌策略
    Cards makeStrategy();
    //1 第一个出牌
    Cards firstPlay();
    //2 得到比指定牌型大的牌
    Cards getGreaterCards(PlayHand hand);
    /******************************************************************/


    //递归找出cards里面所有顺子
    //allSeq存储所有顺子集合的集合（只能选一种）  oneOfSeq存储取出对应顺子后剩下的所有顺子一起的集合
    void pickSeqSingles(const Cards& cards,QVector<QVector<Cards>> &allSeq,const QVector<Cards> &oneOfSeq);
    //筛选最优的顺子集合
    QVector<Cards> pickSeqSignels();

    //3 能大过指定的牌时，判断是出牌还是过
    bool whetherBeat(Cards &cards);

private:
    QVector<Cards> getCards(Card::CardPoint beginPoint,int num);
    QVector<Cards> getThreeSignalOrPair(Card::CardPoint begin,PlayHand::HandType type);
    QVector<Cards> getPlane(Card::CardPoint begin);
    QVector<Cards> getPlane2SignalOr2Pair(Card::CardPoint begin,PlayHand::HandType type);

    using function = Cards (Strategy::*)(Card::CardPoint);
    struct info
    {
        Card::CardPoint begin;
        Card::CardPoint end;
        int extra;
        bool flag;
        int num;
        int baseNum;
        function getSeq;
    };
    QVector<Cards> getSeqSingalOrPair(const info &info);
    Cards getBaseSeqSignal(Card::CardPoint point);
    Cards getBaseSeqPair(Card::CardPoint point);

    QVector<Cards> getBomb(Card::CardPoint begin);

    Player* m_player;
    Cards m_cards;
};

#endif // STRATEGY_H
