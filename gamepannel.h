#ifndef GAMEPANNEL_H
#define GAMEPANNEL_H

#include "alarmclock.h"
#include "cardpannel.h"
#include "gamecontrol.h"
#include "animationwindow.h"
#include "musiccontrol.h"

#include <QLabel>
#include <QMainWindow>

// QT_BEGIN_NAMESPACE
namespace Ui {
class GamePanel;
}

QT_END_NAMESPACE

class GamePanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit GamePanel(QWidget *parent = nullptr);
    ~GamePanel();
    enum AnimationType{ShunZi,LianDui,FeiJi,Bomb,JokerBomb,Bet};

    //分数重置
    void scoreUpdate();

    //实例化游戏控制类
    void gameControl();

    //切割图片
    void initCard();
    //裁剪图片
    void cropImage(QPixmap &pix,int x,int y,Card& c);
    //按钮组
    void initButtonsGroup();
    //玩家上下文初始化
    void initPlayerContext();
    //游戏场景初始化
    void initGameScene();
    //处理游戏状态
    void gameStatusPrecess(GameControl::GameStatus status);
    //发牌
    void startDispatchCard();

    //定时器的处理
    void onDispatchCard();
    //移动扑克牌
    void moveCard(Player* play,int pos);
    //显示扑克牌区域
    void disposCard(Player* play,const Cards &cards);
    //更新扑克牌在窗口的显示
    void updatePlayerCards(Player* player);
    //加载玩家头像
    QPixmap loadRoleImage(Player::Role role,Player::Sex sex,Player::Direction direction);

    //处理玩家游戏状态变化
    void onPlayerStatusChanged(Player* player,GameControl::PlayerStatus status);
    //处理抢地主
    void onGrapLord(Player* player,int point,bool flag);
    //处理出牌
    void onPlayHand(Player* play,const Cards &card);
    //处理卡牌点击
    void onCurrSelectCard(Qt::MouseButton button);
    //处理玩家出牌
    void userPlayHand();
    //处理玩家pass
    void userPass();
    //显示结束面板
    void showEndingPanel();
    //初始化闹钟
    void initAlarmClock();

    //隐藏玩家的牌
    void hidePlayerCards(Player* player);
    //显示动画特效
    void showAnimation(AnimationType type,int bet = 0);

protected:
    void paintEvent(QPaintEvent* ev);

    void mouseMoveEvent(QMouseEvent * ev);

private:
    enum CardAlign{Horizontal,Vertical};
    struct PlayerContext
    {
        //玩家扑克牌显示位置
        QRect cardRect;
        //扑克牌正面还是反面
        bool isForntSide;
        //扑克牌对齐方式 水平/垂直
        CardAlign align;
        //玩家出牌区域
        QRect playHandRect;
        //玩家头像位置
        QLabel* roleImg;
        //游戏过程中的提示信息，比如：不出
        QLabel* info;
        //玩家打出的牌
        Cards lastCards;
    };

    Ui::GamePanel *ui;
    QPixmap m_bkPix;//背景图片
    GameControl* m_gameCtrol;//游戏控制类实例
    QVector<Player*> m_playerList;//存储玩家实例
    QMap<Card,CardPannel*> m_cardMap;//存储卡片信息
    QSize m_cardSize;//卡片大小
    QPixmap m_cardBackImg;//卡片背面图片
    QMap<Player*,PlayerContext> m_contextMap;//存储玩家对应的窗口信息
    CardPannel* m_baseCard;//中心区域扑克牌
    CardPannel* m_moveCard;
    QVector<CardPannel*> m_last3Cards;
    QPoint m_baseCardPoint;//中心扑克牌的位置（点）
    GameControl::GameStatus m_status;
    QTimer * m_time;
    AnimationWindow* m_animation;
    CardPannel* m_currSelectedCard;
    QSet<CardPannel*> m_currSelectCards;
    QRect m_rect;   //手里卡牌的区域
    QHash<CardPannel*,QRect> m_userCards; //卡牌与对应点击区域
    AlarmClock* m_clock;
    MusicControl* m_music;
};
#endif // GAMEPANNEL_H
