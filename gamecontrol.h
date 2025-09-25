#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include "player.h"
#include "robot.h"
#include "userplayer.h"
#include "cards.h"

struct PointCompare
{
    PointCompare()
    {
        reset();
    }
    void reset()
    {
        player = nullptr;
        maxPoint = 0;
        num =0;
    }
    Player* player;
    int maxPoint;
    int num;//第几次叫地主  最大三次
};

class GameControl : public QObject
{
    Q_OBJECT
public:
    //游戏状态
    enum GameStatus{DispatchCard,CallingLord,PlayingHand};
    //玩家状态
    enum PlayerStatus{ThinkingForCallLord,ThinkingForPlayHand,Winning};
    explicit GameControl(QObject *parent = nullptr);

    //初始化玩家
    void playerInit();

    //得到玩家实例
    Robot* getLeftRobot();
    Robot* getRightRobot();
    UserPlayer* getUserPlayer();

    //当前玩家
    void setCurrentPlayer(Player* player);
    Player * getCurrentPlyer();

    //得到出牌玩家和打出的牌
    Player* getPendPlayer();
    Cards getPendCards();

    //初始化扑克牌
    void initAllCards();

    //每次发一张牌
    Card takeOneCard();

    //得到最后的三张牌
    Cards getSurplusCards()
    {
        return m_allCards;
    }

    //重置卡牌数据
    void resetCardData();

    //叫地主/抢地主
    void startLordCall();
    //成为地主
    void becomeLord(Player* player,int bet);
    //返回最大下注点数
    int getMaxBet();

    //清空分数
    void clearPlayerScore();

    //处理叫地主
    void onGrabLordBet(Player* player,int point);
    //处理出牌
    void onPlayHand(Player* player,const Cards& cards);

signals:
    //玩家状态变化
    void playerStatusChanged(Player* play,PlayerStatus status);
    void controlNoticyGrabLord(Player* player,int point,bool flag);
    //游戏状态变化
    void controlGameStatusChange(GameStatus status);
    //告知主界面玩家出牌了
    void controlNoticyPlayHand(Player* player,const Cards& cards);
    //告知其他玩家出牌信息
    void controlNoticyPendInfo(Player* player,const Cards& cards);
private:
    Robot* m_robotLeft = nullptr;
    Robot* m_robotRight = nullptr;
    UserPlayer* m_user = nullptr;
    Player* m_currPlayer = nullptr;
    Player* m_pendPlayer = nullptr;
    Cards m_pendCards;
    Cards m_allCards;
    PointCompare m_pointCompare;
    int  m_bet = 0;
};

#endif // GAMECONTROL_H
