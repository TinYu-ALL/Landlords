#include "strategy.h"

#include <QMap>

#include <QDebug>

Strategy::Strategy(Player *player, const Cards &cards):m_cards(cards)
{
    m_player = player;
}

Cards Strategy::findSamePointCards(Card::CardPoint point, int count)
{
    if(count<1 || count>4)
    {
        return Cards();
    }
    //大小王
    if(point == Card::Card_BJ || point ==Card::Card_SJ)
    {
        if(count>1)
        {
            return Cards();
        }
        Card card;
        card.setPoint(point);
        card.setSuit(Card::Suit_Begin);
        if(m_cards.contains(card))
        {
            Cards cards;
            cards.add(card);
            return cards;
        }
        return Cards();
    }
    //非大小王
    int cardCount = 0;
    Cards findCards;
    for(int suit = Card::Suit_Begin+1;suit!=Card::Suit_End;suit++)
    {
        Card card;
        card.setPoint(point);
        card.setSuit((Card::CardSuit)suit);
        if(m_cards.contains(card))
        {
            findCards.add(card);
            cardCount++;
            if(cardCount == count)
            {
                return findCards;
            }
        }
    }
    return Cards();
}

QVector<Cards> Strategy::findCountCards(int count)
{
    if(count<1 || count>4)
    {
        return QVector<Cards>();
    }
    QVector<Cards> countArray;
    for(auto point = Card::Card_3; point<Card::Card_End; point = (Card::CardPoint)(point+1))
    {
        if(m_cards.pointCount(point)==count)
        {
            Cards cards;
            cards<<findSamePointCards(point,count);
            countArray.push_back(cards);
        }
    }
    return countArray;
}

Cards Strategy::findRangeCards(Card::CardPoint begin, Card::CardPoint end)
{
    Cards rangeCards;
    for(auto point = begin;point<=end;point = (Card::CardPoint)(point+1))
    {
        int num = m_cards.pointCount(point);
        Cards cards = findSamePointCards(point,num);
        rangeCards << cards;
    }
    return rangeCards;
}

QVector<Cards> Strategy::findTypeCards(PlayHand hand, bool flag)
{
    PlayHand::HandType type = hand.getType();
    Card::CardPoint point = hand.getPoint();
    int extra = hand.getExtra();

    //起始点数
    Card::CardPoint beginPoint = flag ? Card::CardPoint(point+1) : Card::Card_3;

    switch (type) {
    case PlayHand::Hand_Signal:
        return getCards(beginPoint,1);
    case PlayHand::Hand_Pair:
        return getCards(beginPoint,2);
    case PlayHand::Hand_Three:
        return getCards(beginPoint,3);
    case PlayHand::Hand_Three_One:
        return getThreeSignalOrPair(beginPoint,PlayHand::Hand_Signal);
    case PlayHand::Hand_Three_Pair:
        return getThreeSignalOrPair(beginPoint,PlayHand::Hand_Pair);
    case PlayHand::Hand_Plane:
        return getPlane(beginPoint);
    case PlayHand::Hand_Plane_signal:
        return getPlane2SignalOr2Pair(beginPoint,PlayHand::Hand_Signal);
    case PlayHand::Hand_Plane_Pair:
        return getPlane2SignalOr2Pair(beginPoint,PlayHand::Hand_Pair);
    case PlayHand::Hand_Seq_Signal:
    {
        info info;
        info.baseNum = 5;
        info.begin = beginPoint;
        info.end = Card::Card_Q;
        info.extra = extra;
        info.flag = flag;
        info.getSeq = &Strategy::getBaseSeqSignal;
        info.num = 1;
        return getSeqSingalOrPair(info);
    }
    case PlayHand::Hand_Seq_Pair:
    {
        info info;
        info.baseNum = 3;
        info.begin = beginPoint;
        info.end = Card::Card_Q;
        info.extra = extra;
        info.flag = flag;
        info.getSeq = &Strategy::getBaseSeqPair;
        info.num = 2;
        return getSeqSingalOrPair(info);
    }
    case PlayHand::Hand_Bomb:
        return getBomb(beginPoint);
    default:
        return QVector<Cards>();
    }
}

Cards Strategy::makeStrategy()
{
    //得到出牌玩家和打出的牌
    Player* pendPlay = m_player->getPendPlayer();
    Cards pendCards = m_player->getPendCards();

    //判断出牌玩家是不是自己
    if(pendPlay == m_player || pendPlay == nullptr)
    {
        //是，没有出牌限制，直接出牌
        return firstPlay();
    }else
    {
        //不是，找比这个牌大的牌
        PlayHand type(pendCards);
        Cards beatCards = getGreaterCards(type);
        //考虑是否出牌
        bool whether = whetherBeat(beatCards);
        if(whether)
        {
            return beatCards;
        }else
        {
            return Cards();
        }
    }
    return Cards();
}

Cards Strategy::firstPlay()
{
    //判断是否只剩单一的牌型
    PlayHand hand(m_cards);
    if(hand.getType()!=PlayHand::Hand_UnKonw) return m_cards;
    //不是单一的
    //判断是否有顺子
    QVector<Cards> seqArray = pickSeqSignels();
    if(!seqArray.isEmpty())
    {
        //单牌数量
        int baseNum = findCountCards(1).size();
        //删除顺子
        Cards copy = m_cards;
        copy.remove(seqArray);
        //判断是否删除后单牌数量少于删除前
        int rmNum = Strategy(m_player,copy).findCountCards(1).size();
        if(baseNum > rmNum) return seqArray[0];
    }

    bool hasPlane,hasThree,hasPair;
    hasPlane = hasThree = hasPair = false;
    Cards copyCards = m_cards;

    //有没有炸弹
    QVector<Cards> bombArray = findTypeCards(PlayHand(PlayHand::Hand_Bomb,Card::Card_Begin,0),false);
    copyCards.remove(bombArray);

    //有没有飞机
    QVector<Cards> planeArray = Strategy(m_player,copyCards).findTypeCards(PlayHand(PlayHand::Hand_Plane,Card::Card_Begin,0),false);
    if(!planeArray.isEmpty())
    {
        hasPlane = true;
        copyCards.remove(planeArray);
    }

    //有没有三张的
    QVector<Cards> threeArray = Strategy(m_player,copyCards).findTypeCards(PlayHand(PlayHand::Hand_Three,Card::Card_Begin,0),false);
    if(!threeArray.isEmpty())
    {
        hasThree = true;
        copyCards.remove(threeArray);
    }

    //有没有连对
    QVector<Cards> pairArray = Strategy(m_player,copyCards).findTypeCards(PlayHand(PlayHand::Hand_Seq_Pair,Card::Card_Begin,0),false);
    if(!pairArray.isEmpty())
    {
        hasPair = true;
        copyCards.remove(pairArray);
    }

    if(hasPair)
    {
        Cards maxCards;
        for(int i = 0 ;i<pairArray.size();++i)
        {
            if(maxCards.cardCount() < pairArray[i].cardCount())
            {
                maxCards = pairArray[i];
            }
        }
        return maxCards;
    }

    if(hasPlane)
    {
        //飞机带两对
        bool twoPair = false;
        QVector<Cards> pairArray2;
        for(auto point = Card::Card_3;point!= Card::Card_Q;point = Card::CardPoint(point+1))
        {
            Cards pair = findSamePointCards(point,2);
            if(!pair.isEmpty())
            {
                pairArray2.push_back(pair);
                if(pairArray2.size() == 2)
                {
                    twoPair = true;
                    break;
                }
            }
        }
        if(twoPair)
        {
            Cards temp = planeArray[0];
            temp.add(pairArray2);
            return temp;
        }
        else
        {
            //飞机带两单
            bool twoSignels = false;
            QVector<Cards> signelsArray;
            for(auto point = Card::Card_3;point!= Card::Card_Q;point = Card::CardPoint(point+1))
            {
                Cards pair = findSamePointCards(point,1);
                if(!pair.isEmpty())
                {
                    signelsArray.push_back(pair);
                    if(signelsArray.size() == 2)
                    {
                        twoSignels = true;
                        break;
                    }
                }
            }
            if(twoSignels)
            {
                Cards temp = planeArray[0];
                temp.add(signelsArray);
                return temp;
            }else
            {
                //飞机
                return planeArray[0];
            }
        }
    }

    if(hasThree)
    {
        if(PlayHand(threeArray[0]).getPoint() < Card::Card_A)
        {
            for(auto point = Card::Card_3;point!= Card::Card_A;point = Card::CardPoint(point+1))
            {
                int num = copyCards.pointCount(point);
                if(num == 1)
                {
                    Cards signel = Strategy(m_player,copyCards).findSamePointCards(point,1);
                    Cards temp = threeArray[0];
                    temp.add(signel);
                    return temp;
                }else if(num == 2)
                {
                    Cards pair = Strategy(m_player,copyCards).findSamePointCards(point,2);
                    Cards temp = threeArray[0];
                    temp.add(pair);
                    return temp;
                }
            }
        }
        return threeArray[0];
    }

    //单或对
    Player* nextPlayer = m_player->getNextPlayer();
    if(nextPlayer->getRole()!=m_player->getRole() && nextPlayer->getCards().cardCount() <= 2)
    {
        for(auto point = Card::CardPoint(Card::Card_End-1);point>=Card::Card_3;point=Card::CardPoint(point-1))
        {
            int num = copyCards.pointCount(point);
            if(num == 1)
            {
                Cards signel = Strategy(m_player,copyCards).findSamePointCards(point,1);
                return signel;
            }else if(num == 2)
            {
                Cards pair = Strategy(m_player,copyCards).findSamePointCards(point,1);
                return pair;
            }
        }
    }else
    {
        for(auto point = Card::Card_3;point!=Card::Card_End;point=Card::CardPoint(point+1))
        {
            int num = copyCards.pointCount(point);
            if(num == 1)
            {
                Cards signel = Strategy(m_player,copyCards).findSamePointCards(point,1);
                return signel;
            }else if(num == 2)
            {
                Cards pair = Strategy(m_player,copyCards).findSamePointCards(point,1);
                return pair;
            }
        }
    }

    return Cards();
}


/*
开始
├─ 判断出牌方是否对手 & 对手牌数 ≤3？(3:对手可能“一手清”)
│   ├─ 是 → 找炸弹/王炸 → 找到则返回，否则继续
│   └─ 否 → 继续
├─ 剔除顺子（优化计算）
├─ 尝试用非顺子牌组找压制牌型
│   ├─ 找到 → 返回最优解（下家是对手&下家牌数 ≤2返回最大牌组，否则返回最小牌组）(2:下家可能“立刻赢”)
│   └─ 没找到 → 继续
├─ 尝试用原始牌组（含顺子）找压制牌型
│   ├─ 找到 → 返回最优解（下家是对手&下家牌数 ≤2返回最大牌组，否则返回最小牌组）
│   └─ 没找到 → 返回空牌组
结束
*/
Cards Strategy::getGreaterCards(PlayHand hand)
{
    //比指定牌型大的牌分为 找炸弹和王炸\找对应的牌
    Player* pendPlayer = m_player->getPendPlayer();
    //找炸弹和王炸    出牌玩家和当前玩家不是队友并且出牌玩家手里的牌小于3张
    if(pendPlayer != nullptr && pendPlayer->getRole()!=m_player->getRole() && pendPlayer->getCards().cardCount()<=3)
    {
        QVector<Cards> bomb = findCountCards(4);
        //炸弹能不能压住
        for(int i = 0;i<bomb.size();++i)
        {
            if(PlayHand(bomb[i]).canBeat(hand))
            {
                return bomb[i];
            }
        }
        //有没有王炸
        Cards sj = findSamePointCards(Card::Card_SJ,1);
        Cards bj = findSamePointCards(Card::Card_BJ,1);
        if(!sj.isEmpty() && !bj.isEmpty())
        {
            Cards jokers;
            jokers << sj << bj;
            return jokers;
        }
    }
    //找对应牌型的牌
    //当前玩家和下一个玩家不是队友(出较大的还是较小的牌)
    Player* nextPlayer = m_player->getNextPlayer();

    //剔除顺子  优先尝试用更高效的牌型（如炸弹、三带）压制对手，同时避免拆解复杂牌型
    Cards copy = m_cards;
    copy.remove(Strategy(m_player,copy).pickSeqSignels());

    auto beatCards = std::bind([=](const Cards& cards){
        QVector<Cards> beatArray = Strategy(m_player,cards).findTypeCards(hand,true);
        if(!beatArray.isEmpty())
        {
            if(nextPlayer->getRole()!=m_player->getRole() && nextPlayer->getCards().cardCount()<=2)
            {
                return beatArray.back();
            }else return beatArray.front();
        }
        return Cards();
    },std::placeholders::_1);

    Cards cs;
    if(!(cs = beatCards(copy)).isEmpty())
    {
        return  cs;
    }else
    {
        if(!(cs = beatCards(m_cards)).isEmpty()) return cs;
    }
    return Cards();
}

void Strategy::pickSeqSingles(const Cards &cards,QVector<QVector<Cards>> &allSeq,const QVector<Cards> &oneOfSeq)
{
    //得到所有顺子组合
    QVector<Cards> allSeqSignel = Strategy(m_player,cards).findTypeCards(PlayHand(PlayHand::Hand_Seq_Signal,Card::Card_Begin,0),false);
    //结束递归
    if(allSeqSignel.isEmpty())
    {
        allSeq << oneOfSeq;
    }
    //对顺子进行遍历
    else
    {
        Cards copyCards = cards;
        for(int i=0;i<allSeqSignel.size();++i)
        {
            //取出顺子
            Cards oneOfCards = allSeqSignel.at(i);
            //将顺子从原卡中删除
            Cards temp = copyCards;
            temp.remove(oneOfCards);

            QVector<Cards> seqArray = oneOfSeq;
            seqArray<<oneOfCards;

            //检测还有没有其他的顺子
            pickSeqSingles(temp,allSeq,seqArray);
        }
    }

}

QVector<Cards> Strategy::pickSeqSignels()
{
    QVector<QVector<Cards>> allSeq; //存储oneOfSeq的集合
    QVector<Cards> oneOfSeq;    //没有重复卡牌的顺子的集合（手里可以打出几个顺子）
    //排除炸弹，三张的
    Cards copy = m_cards;
    copy.remove(findCountCards(4));
    copy.remove(findCountCards(3));
    pickSeqSingles(copy,allSeq,oneOfSeq);

    if(allSeq.isEmpty()) return QVector<Cards>();


    QMap<int,int> seqMarks;
    for(int i=0;i<allSeq.size();++i)
    {
        Cards copyCards = m_cards;

        QVector<Cards> seqArray = allSeq.at(i);
        copyCards.remove(seqArray);

        //判断剩下的单牌数量，越少越好
        QVector<Cards> signelCards = Strategy(m_player,copyCards).findCountCards(1);
        //转为QVector<Card>类型的
        CardList cardlist;
        for(int j = 0;j<signelCards.size();++j)
        {
            cardlist << signelCards[j].toCardList();
        }
        //找点数相对较大的顺子==> 单牌点数和最小
        int mark = 0;
        for(int j = 0;j<cardlist.size();++j)
        {
            mark += cardlist[j].Point()+15; //排除 多张小牌优于少张大牌 的情况
        }
        seqMarks.insert(i,mark);
    }

    int minMark = 9999;
    int value = 0;
    for(auto it = seqMarks.begin();it!=seqMarks.end();++it)
    {
        if(it.value()< minMark)
        {
            minMark = it.value();
            value = it.key();
        }
    }

    return allSeq.at(value);
}

bool Strategy::whetherBeat(Cards &cards)
{
    //没找到压住的牌
    if(cards.isEmpty()) return false;

    //得到出牌玩家和打出的牌
    Player* pendPlayer = m_player->getPendPlayer();
    Cards pendCards = m_player->getPendCards();
    //判断出牌玩家和自己是不是一伙的(都是农民)
    if(pendPlayer->getRole() == m_player->getRole())
    {
        //1、打出去后手里的剩下的牌没多少，并且只有一个牌型了 ==> 出牌
        Cards copy = m_cards;
        copy.remove(cards);
        if(PlayHand(copy).getType() != PlayHand::Hand_UnKonw) return true;

        //最小点数的牌
        Card::CardPoint basePoint = PlayHand(cards).getPoint();
        //2、要打出去的牌最小是2，大小王 ==> 不出牌
        if(basePoint == Card::Card_2 || basePoint == Card::Card_SJ || basePoint == Card::Card_BJ)
        {
            return false;
        }

        //3、队友出炸弹时 ==> 不出
        if(PlayHand(pendCards).getType() == PlayHand::Hand_Bomb)
        {
            return false;
        }
    }else
    {
        // 要出的牌是三个2 ==> 不出
        PlayHand myHand(cards);
        if((myHand.getType()==PlayHand::Hand_Three || myHand.getType()==PlayHand::Hand_Three_One
             || myHand.getType()==PlayHand::Hand_Three_Pair) && myHand.getPoint()==Card::Card_2)
        {
            return false;
        }
        //打出的牌时对2，出牌玩家手里的牌>=10&&自己手里的牌<=5 ==>不出
        if(myHand.getType()==PlayHand::Hand_Pair && myHand.getPoint() == Card::Card_2
            && pendPlayer->getCards().cardCount()>=10 && m_cards.cardCount() <=5)
        {
            return false;
        }
    }
    return true;
}

QVector<Cards> Strategy::getCards(Card::CardPoint beginPoint, int num)
{
    QVector<Cards> cardArray;
    for(auto point = beginPoint; point<Card::Card_End; point=(Card::CardPoint)(point+1))
    {
        //不拆分别的牌型
        if(m_cards.pointCount(point) == num)
        {
            Cards cards = findSamePointCards(point,num);
            cardArray.push_back(cards);
        }
    }
    return cardArray;
}

QVector<Cards> Strategy::getThreeSignalOrPair(Card::CardPoint begin, PlayHand::HandType type)
{
    //找到点数相同的三张牌
    QVector<Cards> findArray = getCards(begin,3);
    if(!findArray.isEmpty())
    {
        //将找到的牌从牌组中删除
        Cards cd = m_cards;
        cd.remove(findArray);
        //找对应的牌型的牌
        Strategy st(m_player,cd);
        QVector<Cards> cv = st.findTypeCards(PlayHand(type,Card::Card_Begin,0),false);
        if(!cv.isEmpty())
        {
            //组合到一起
            for(int i = 0;i<findArray.size();++i)
            {
                findArray[i].add(cv.at(i));
            }
        }else
        {
            findArray.clear();
        }
    }
    //返回结果
    return findArray;
}

QVector<Cards> Strategy::getPlane(Card::CardPoint begin)
{
    QVector<Cards> mc;
    for(auto point = begin;point<=Card::Card_K;point = (Card::CardPoint)(point+1))
    {
        //根据点数和数量找牌
        Cards prvCards = findSamePointCards(point,3);
        Cards nextCards = findSamePointCards((Card::CardPoint)(point+1),3);
        if(!prvCards.isEmpty() && !nextCards.isEmpty())
        {
            Cards temp;
            temp << prvCards << nextCards;
            mc << temp;
        }
    }
    return mc;
}

QVector<Cards> Strategy::getPlane2SignalOr2Pair(Card::CardPoint begin, PlayHand::HandType type)
{
    //找到飞机
    QVector<Cards> findArray = getPlane(begin);
    if(!findArray.isEmpty())
    {
        //将找到的牌从牌组中删除
        Cards cd = m_cards;
        cd.remove(findArray);
        //找对应的牌型的牌
        Strategy st(m_player,cd);
        QVector<Cards> cv = st.findTypeCards(PlayHand(type,Card::Card_Begin,0),false);
        if(cv.size()>=2)
        {
            //组合到一起
            for(int i = 0;i<findArray.size();++i)
            {
                Cards temp;
                temp << cv[0] << cv[1];
                findArray[i].add(temp);
            }
        }else
        {
            findArray.clear();
        }
    }
    //返回结果
    return findArray;
}

Cards Strategy::getBaseSeqSignal(Card::CardPoint point)
{
    Cards card0 = findSamePointCards(point,1);
    Cards card1 = findSamePointCards(Card::CardPoint(point+1),1);
    Cards card2 = findSamePointCards(Card::CardPoint(point+2),1);
    Cards card3 = findSamePointCards(Card::CardPoint(point+3),1);
    Cards card4 = findSamePointCards(Card::CardPoint(point+4),1);
    Cards cards;
    if(!card0.isEmpty() && !card1.isEmpty() && !card2.isEmpty() && !card3.isEmpty() && !card4.isEmpty())
    {
        cards << card0 << card1 << card2 << card3 << card4;
    }
    return cards;
}

Cards Strategy::getBaseSeqPair(Card::CardPoint point)
{
    Cards card0 = findSamePointCards(point,2);
    Cards card1 = findSamePointCards(Card::CardPoint(point+1),2);
    Cards card2 = findSamePointCards(Card::CardPoint(point+2),2);
    Cards cards;
    if(!card0.isEmpty() && !card1.isEmpty() && !card2.isEmpty())
    {
        cards << card0 << card1 << card2;
    }
    return cards;
}

QVector<Cards> Strategy::getBomb(Card::CardPoint begin)
{
    QVector<Cards> cardsArray;
    for(auto point = begin;point!= Card::Card_End;point = (Card::CardPoint)(point+1))
    {
        Cards cards = findSamePointCards(point,4);
        if(!cards.isEmpty())
        {
            cardsArray << cards;
        }
    }
    return cardsArray;
}

QVector<Cards> Strategy::getSeqSingalOrPair(const info &info)
{
    QVector<Cards> findCardsArray;
    //是不是要比指定的牌大，大就要求牌数量一致extra
    if(info.flag)
    {
        //最少三个，最大卡牌为A
        for(auto point = info.begin; point<info.end; point=(Card::CardPoint)(point+1))
        {
            bool findFlag = true;//有没有找到这个点数对应的对子
            Cards seqCards;
            //基于这个点开始找extra数量的牌
            for(int i =0;i<info.extra;++i)
            {
                //基于点数和数量找
                Cards cards = findSamePointCards((Card::CardPoint)(point+i),info.num);
                //没找到就不满足要求（不连续，extra数量不够）直接下一个点数开始找
                if(cards.isEmpty()||(Card::CardPoint)(point+info.extra)>=Card::Card_2)
                {
                    findFlag = false;
                    seqCards.clear();
                    break;
                }else
                {
                    seqCards << cards;
                }
            }
            if(findFlag)
            {
                findCardsArray << seqCards;
                return findCardsArray;
            }
        }
    }else
    {
        for(auto point = info.begin; point<info.end; point=(Card::CardPoint)(point+1))
        {
            //存储基础三对
            Cards baseCards = (this->*info.getSeq)(point);
            if(baseCards.isEmpty()) continue;
            findCardsArray << baseCards;

            //继续找有没有连续的对
            int baseNum = info.baseNum;
            //存储后续满足条件的对
            Cards nextCards;
            while(true)
            {
                //新的起点
                Card::CardPoint newBegin = (Card::CardPoint)(point+baseNum);
                //判断是否超出上限
                if(newBegin>=Card::Card_2) break;
                //查找满足条件的对
                Cards newCards = findSamePointCards(newBegin,info.num);
                if(newCards.isEmpty())
                {
                    //没找到就不连续直接退出循环
                    break;
                }else
                {
                    //findCardsArray存储三对、四对……
                    nextCards << newCards;
                    Cards cards = baseCards;
                    cards << nextCards;
                    findCardsArray << cards;
                    baseNum++;
                }
            }
        }
    }
    return findCardsArray;
}
