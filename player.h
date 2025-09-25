#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "cards.h"

class Player : public QObject
{
    Q_OBJECT
public:
    enum Role{Lord,Farmer};    //角色
    enum Sex{Man,Woman};//性别
    enum Direction{Left,Right};//头像的显示方位
    enum Type{Robot,User,Unknow};//玩家的类型
    explicit Player(QObject *parent = nullptr);

    explicit Player(QString name,QObject *parent = nullptr);

    //名字
    void setName(QString name);
    QString getName();

    //角色
    void setRole(Role role);
    Role getRole();

    //性别
    void setSex(Sex sex);
    Sex getSex();

    //头像显示方位
    void setDirection(Direction direction);
    Direction getDirection();

    //玩家类型
    void setType(Type type);
    Type getType();

    //玩家分数
    void setScore(int score);
    int getSocre();

    //游戏结果
    void setWin(bool flag);
    bool isWin();

    //上下家
    void setPrevPLayer(Player* prev);
    void setNextPlayer(Player* next);
    Player* getPrevPlayer();
    Player* getNextPlayer();

    //叫地主/抢地主
    void grabLordBet(int point);

    //存储扑克牌
    void storeDispatchCard(const Card& card);
    void storeDispatchCard(const Cards& cards);

    //出牌
    void playHand(const Cards& cards);

    //得到所有牌
    Cards getCards();

    //清空手里的牌
    void clearCards();

    //设置出牌玩家和已经打出的牌
    void setPendingInfo(Player* player,const Cards& cards);
    Player* getPendPlayer();
    Cards getPendCards();

    //虚函数（用来管理子类的叫地主和出牌）
    virtual void prepareCallLord();
    virtual void preparePlayHand();
    virtual void thinkCallLord();
    virtual void thinkPlayHand();

signals:
    //通知已经叫地主下注
    void noticyGrabLordBet(Player* play,int point);

    //通知已经出牌
    void noticyPlayHand(Player* player,const Cards& cards);

    //通知主界面存储牌了
    void noticyStoreCard(Player* player,const Cards& cards);

protected:
    int m_score = 0;
    QString m_name;
    Role m_role;
    Sex m_sex;
    Direction m_direction;
    Type m_type;
    bool m_isWin = false;
    Player* m_prev = nullptr;
    Player* m_next = nullptr;
    Cards m_cards;
    Player* m_pendPlayer = nullptr;
    Cards m_pendCards;

};

#endif // PLAYER_H
