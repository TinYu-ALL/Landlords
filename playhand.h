#ifndef PLAYHAND_H
#define PLAYHAND_H

#include "Card.h"
#include "cards.h"
class PlayHand
{
public:
    //出牌组合
    enum HandType
    {
        Hand_UnKonw,                    //未知
        Hand_Pass,                      //过

        Hand_Signal,                    //单
        Hand_Pair,                      //双

        Hand_Three,                     //三个
        Hand_Three_One,                 //三带一
        Hand_Three_Pair,                //三带二

        Hand_Plane,                     //飞机    555_666
        Hand_Plane_signal,              //飞机带单 555_666_3_4
        Hand_Plane_Pair,                //飞机带双 555_666_33_44

        Hand_Seq_Signal,                //顺子    34567……
        Hand_Seq_Pair,                  //连对    33_44_55……

        Hand_Bomb,                      //炸弹
        Hand_Bomb_Signal,               //炸弹带单
        Hand_Bomb_Two_Signal,           //炸弹带两单
        Hand_Bomb_Pair,                 //炸弹带双

        Hand_Bomb_Jokers,               //王炸
        Hand_Bomb_Jokers_Signal,        //王炸带单
        Hand_Bomb_Jokers_Two_Signal,    //王炸带俩单
        Hand_Bomb_Jokers_Pair           //王炸带双
    };

    PlayHand();
    //传递一组牌，判断牌型、点数、额外信息(顺子连续几张，连对几对)
    explicit PlayHand(const Cards& cards);
    //存储信息
    PlayHand(HandType type,Card::CardPoint point,int extra);

    //返回信息
    //返回牌的类型
    HandType getType();
    //返回牌的最小点数
    Card::CardPoint getPoint();
    int getExtra();

    //比较牌型
    bool canBeat(const PlayHand& other);

private:
    //对扑克牌进行分类：1张，2张，3张，4张
    void classify(const Cards& card);
    //牌型分类
    void cardType();
    //判断牌的类型
    bool isPass();                  //放弃出牌
    bool isSiganl();                //单
    bool isPair();                  //双
    bool isThree();                 //三个
    bool isThreeSignal();           //三带一
    bool isThreePair();             //三带二
    bool isPlane();                 //飞机   333444     相邻且不能有222
    bool isPlaneTwoSignal();        //飞机带两单         不能带大小王
    bool isPlaneTwoPair();          //飞机带两对
    bool isSeqSignal();             //顺子              连续大于5张，顺子小于2（……、9、10、j、q、k）
    bool isSeqPair();               //连对              连续3对以上，对子小于22
    bool isBomb();                  //炸弹
    bool isBombSignal();            //炸弹带一单
    bool isBombPair();              //炸弹带一对
    bool isBombTwoSignal();         //炸弹带两单
    bool isBombJokers();            //王炸
    bool isBombJokersSignal();      //王炸带一单
    bool isBombJokersPair();       //王炸带一对
    bool isBombJokersTwoSignal();  //王炸带双单



private:
    HandType m_handType;
    Card::CardPoint m_point;
    int m_extra;
    QVector<Card::CardPoint> m_oneCard;     //存储一张的卡牌的点数
    QVector<Card::CardPoint> m_twoCards;
    QVector<Card::CardPoint> m_threeCards;
    QVector<Card::CardPoint> m_fourCards;
};

#endif // PLAYHAND_H
