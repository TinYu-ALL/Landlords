#include "playhand.h"

PlayHand::PlayHand() {}

PlayHand::PlayHand(const Cards &cards)
{
    //对扑克牌进行分类：1张，2张，3张，4张
    classify(cards);
    //牌型分类
    cardType();
}

PlayHand::PlayHand(HandType type, Card::CardPoint point, int extra)
{
    m_handType = type;
    m_point = point;
    m_extra = extra;
}

void PlayHand::classify(const Cards &card)
{
    Cards cards = const_cast<Cards&>(card);
    CardList list = cards.toCardList();
    int cardsNum[Card::Card_End];
    memset(cardsNum,0,sizeof(cardsNum));
    for(int i = 0;i<list.size();i++)
    {
        Card c = list.at(i);
        cardsNum[c.Point()]++;
    }

    m_oneCard.clear();
    m_twoCards.clear();
    m_threeCards.clear();
    m_fourCards.clear();

    for(int j = 0;j<Card::Card_End;j++)
    {
        if(cardsNum[j]==1)
        {
            m_oneCard.push_back((Card::CardPoint)j);
        }else if(cardsNum[j]==2)
        {
            m_twoCards.push_back((Card::CardPoint)j);
        }else if(cardsNum[j]==3)
        {
            m_threeCards.push_back((Card::CardPoint)j);
        }else if(cardsNum[j]==4)
        {
            m_fourCards.push_back((Card::CardPoint)j);
        }
    }
}

void PlayHand::cardType()
{
    m_handType = Hand_UnKonw;
    m_point = Card::Card_Begin;
    m_extra = 0;

    if(isPass())
    {
        m_handType = Hand_Pass;
    }
    else if(isSiganl())
    {
        m_handType = Hand_Signal;
        m_point = m_oneCard[0];
    }else if(isPair())
    {
        m_handType = Hand_Pair;
        m_point = m_twoCards[0];
    }else if(isThree())
    {
        m_handType = Hand_Three;
        m_point = m_threeCards[0];
    }else if(isThreeSignal())
    {
        m_handType = Hand_Three_One;
        m_point = m_threeCards[0];
    }else if(isThreePair())
    {
        m_handType = Hand_Three_Pair;
        m_point = m_threeCards[0];
    }else if(isPlane())
    {
        m_handType = Hand_Plane;
        //记录点数最小的牌
        m_point = m_threeCards[0];
    }else if(isPlaneTwoSignal())
    {
        m_handType = Hand_Plane_signal;
        //记录点数最小的牌
        m_point = m_threeCards[0];
    }else if(isPlaneTwoPair())
    {
        m_handType = Hand_Plane_Pair;
        //记录点数最小的牌
        m_point = m_threeCards[0];
    }else if(isSeqSignal())
    {
        m_handType = Hand_Seq_Signal;
        //记录点数最小的牌
        m_point = m_oneCard[0];
        m_extra = m_oneCard.size();
    }else if(isSeqPair())
    {
        m_handType = Hand_Seq_Pair;
        //记录点数最小的牌
        m_point = m_twoCards[0];
        m_extra = m_twoCards.size();
    }else if(isBomb())
    {
        m_handType = Hand_Bomb;
        //记录点数最小的牌
        m_point = m_fourCards[0];
    }else if(isBombSignal())
    {
        m_handType = Hand_Bomb_Signal;
        //记录点数最小的牌
        m_point = m_fourCards[0];
    }else if(isBombTwoSignal())
    {
        m_handType = Hand_Bomb_Two_Signal;
        //记录点数最小的牌
        m_point = m_fourCards[0];
    }else if(isBombPair())
    {
        m_handType = Hand_Bomb_Pair;
        //记录点数最小的牌
        m_point = m_fourCards[0];
    }else if(isBombJokers())
    {
        m_handType = Hand_Bomb_Jokers;
    }else if(isBombJokersSignal())
    {
        m_handType = Hand_Bomb_Jokers_Signal;
    }else if(isBombJokersPair())
    {
        m_handType = Hand_Bomb_Jokers_Pair;
    }else if(isBombJokersTwoSignal())
    {
        m_handType = Hand_Bomb_Jokers_Two_Signal;
    }
}

bool PlayHand::isPass()
{
    if(m_oneCard.isEmpty() && m_twoCards.isEmpty() && m_threeCards.isEmpty() && m_fourCards.isEmpty())
    {
        return true;
    }
    return false;
}

Card::CardPoint PlayHand::getPoint()
{
    return m_point;
}

int PlayHand::getExtra()
{
    return m_extra;
}

bool PlayHand::canBeat(const PlayHand &other)
{
    //我的牌型未知
    if(m_handType == Hand_UnKonw)
    {
        return false;
    }
    //对方放弃出牌
    if(other.m_handType==Hand_Pass)
    {
        return true;
    }
    //我王炸
    if(m_handType == Hand_Bomb_Jokers)
    {
        return true;
    }
    if(m_handType == Hand_Bomb && other.m_handType>=Hand_Signal && other.m_handType<=Hand_Seq_Pair)
    {
        return true;
    }
    //牌型一致
    if(m_handType == other.m_handType)
    {
        if(m_handType == Hand_Seq_Signal || m_handType ==Hand_Seq_Pair)
        {
            return m_point>other.m_point && m_extra == other.m_extra;
        }
        else
        {
            return m_point>other.m_point;
        }
    }
    return false;
}

bool PlayHand::isSiganl()
{
    if(m_oneCard.size()==1 && m_twoCards.isEmpty() && m_threeCards.isEmpty() && m_fourCards.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isPair()
{
    if(m_oneCard.isEmpty() && m_twoCards.size()==1 && m_threeCards.isEmpty() && m_fourCards.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isThree()
{
    if(m_oneCard.isEmpty() && m_twoCards.isEmpty() && m_threeCards.size()==1 && m_fourCards.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isThreeSignal()
{
    if(m_oneCard.size()==1 && m_twoCards.isEmpty() && m_threeCards.size()==1 && m_fourCards.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isThreePair()
{
    if(m_oneCard.isEmpty() && m_twoCards.size()==1 && m_threeCards.size()==1 && m_fourCards.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isPlane()
{
    if(m_oneCard.isEmpty() && m_twoCards.isEmpty() && m_threeCards.size()==2 && m_fourCards.isEmpty())
    {
        std::sort(m_threeCards.begin(),m_threeCards.end());
        if(m_threeCards[1]-m_threeCards[0]==1 && m_threeCards[1]<Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoSignal()
{
    if(m_oneCard.size()==2 && m_twoCards.isEmpty() && m_threeCards.size()==2 && m_fourCards.isEmpty())
    {
        std::sort(m_threeCards.begin(),m_threeCards.end());
        std::sort(m_oneCard.begin(),m_oneCard.end());
        if(m_threeCards[1]-m_threeCards[0]==1 && m_threeCards[1]<Card::Card_2 &&
            m_oneCard[0]!=Card::Card_SJ && m_oneCard[1]!=Card::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoPair()
{
    if(m_oneCard.isEmpty() && m_twoCards.size()==2 && m_threeCards.size()==2 && m_fourCards.isEmpty())
    {
        std::sort(m_threeCards.begin(),m_threeCards.end());
        if(m_threeCards[1]-m_threeCards[0]==1 && m_threeCards[1]<Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqSignal()
{
    if(m_oneCard.size()>=5 && m_twoCards.isEmpty() && m_threeCards.isEmpty() && m_fourCards.isEmpty())
    {
        std::sort(m_oneCard.begin(),m_oneCard.end());
        if(m_oneCard.last()-m_oneCard.first()==(m_oneCard.size()-1) &&
            m_oneCard.last()>=Card::Card_3 && m_oneCard.last()<Card::Card_2)                      ////////////////
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqPair()
{
    if(m_oneCard.isEmpty() && m_twoCards.size()>=3 && m_threeCards.isEmpty() && m_fourCards.isEmpty())
    {
        std::sort(m_twoCards.begin(),m_twoCards.end());
        if(m_twoCards.last()-m_twoCards.first()==(m_twoCards.size()-1) &&
            m_twoCards.last()>=Card::Card_3 && m_twoCards.last()<Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBomb()
{
    if(m_oneCard.isEmpty() && m_twoCards.isEmpty() && m_threeCards.isEmpty() && m_fourCards.size()==1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombSignal()
{
    if(m_oneCard.size()==1 && m_twoCards.isEmpty() && m_threeCards.isEmpty() && m_fourCards.size()==1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombPair()
{
    if(m_oneCard.isEmpty() && m_twoCards.size()==1 && m_threeCards.isEmpty() && m_fourCards.size()==1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombTwoSignal()
{
    if(m_oneCard.size()==2 && m_twoCards.isEmpty() && m_threeCards.isEmpty() && m_fourCards.size()==1)
    {
        std::sort(m_oneCard.begin(),m_oneCard.end());
        if(m_oneCard[0]!=Card::Card_SJ && m_oneCard[1]!=Card::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokers()
{
    if(m_oneCard.size()==2 && m_twoCards.isEmpty() && m_threeCards.isEmpty() && m_fourCards.isEmpty())
    {
        std::sort(m_oneCard.begin(),m_oneCard.end());
        if(m_oneCard[0]==Card::Card_SJ && m_oneCard[1]==Card::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersSignal()
{
    if(m_oneCard.size()==3 && m_twoCards.isEmpty() && m_threeCards.isEmpty() && m_fourCards.isEmpty())
    {
        std::sort(m_oneCard.begin(),m_oneCard.end());
        if(m_oneCard[1]==Card::Card_SJ && m_oneCard[2]==Card::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersPair()
{
    if(m_oneCard.size()==2 && m_twoCards.size()==1 && m_threeCards.isEmpty() && m_fourCards.size()==1)
    {
        std::sort(m_oneCard.begin(),m_oneCard.end());
        if(m_oneCard[0]==Card::Card_SJ && m_oneCard[1]==Card::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersTwoSignal()
{
    if(m_oneCard.size()==4 && m_twoCards.isEmpty() && m_threeCards.isEmpty() && m_fourCards.size()==1)
    {
        std::sort(m_oneCard.begin(),m_oneCard.end());
        if(m_oneCard[2]==Card::Card_SJ && m_oneCard[3]==Card::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

PlayHand::HandType PlayHand::getType()
{
    return m_handType;
}
