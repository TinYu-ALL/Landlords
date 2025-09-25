#include "cardpannel.h"
#include "endingpanel.h"
#include "gamepannel.h"
#include "playhand.h"
#include "ui_gamepannel.h"

#include <QPainter>
#include <QRandomGenerator>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QPropertyAnimation>

GamePanel::GamePanel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GamePanel)
{
    ui->setupUi(this);

    //背景图
    int num = QRandomGenerator::global()->bounded(10);
    QString path = QString(":/images/background-%1.png").arg(num+1);
    m_bkPix.load(path);

    //窗口图标和大小
    this->setWindowTitle("欢乐斗地主");
    this->setWindowIcon(QIcon(":/images/logo.ico"));
    this->setFixedSize(1000,650);

    //游戏控制类对象
    gameControl();

    //分数窗口(重置)
    scoreUpdate();

    //切割游戏图片
    initCard();

    //初始化按钮组
    initButtonsGroup();

    //玩家上下文初始化
    initPlayerContext();

    //游戏场景初始化
    initGameScene();

    //初始化闹钟
    initAlarmClock();

    //启动定时器
    m_time = new QTimer(this);
    connect(m_time,&QTimer::timeout,this,&GamePanel::onDispatchCard);

    m_animation = new AnimationWindow(this);

    m_music = new MusicControl(this);
}

GamePanel::~GamePanel()
{
    delete ui;
}

void GamePanel::scoreUpdate()
{
    ui->scorePannel->setScore(m_playerList[0]->getSocre(),
        m_playerList[1]->getSocre(),
        m_playerList[2]->getSocre()
    );
}

void GamePanel::gameControl()
{
    m_gameCtrol = new GameControl(this);
    m_gameCtrol->playerInit();
    //三个玩家实例
    Robot* leftRobot = m_gameCtrol->getLeftRobot();
    Robot* rightRobot = m_gameCtrol->getRightRobot();
    UserPlayer* user = m_gameCtrol->getUserPlayer();
    //存储顺序：左侧机器人，右侧机器人，当前玩家
    m_playerList << leftRobot<<rightRobot<<user;

    //玩家状态改变
    connect(m_gameCtrol,&GameControl::playerStatusChanged,this,&GamePanel::onPlayerStatusChanged);
    //处理每个玩家叫地主 窗口显示信息
    connect(m_gameCtrol,&GameControl::controlNoticyGrabLord,this,&GamePanel::onGrapLord);
    //游戏状态改变
    connect(m_gameCtrol,&GameControl::controlGameStatusChange,this,&GamePanel::gameStatusPrecess);
    //处理玩家出牌
    connect(m_gameCtrol,&GameControl::controlNoticyPlayHand,this,&GamePanel::onPlayHand);

    //处理玩家存牌
    connect(leftRobot,&Player::noticyStoreCard,this,&GamePanel::disposCard);
    connect(rightRobot,&Player::noticyStoreCard,this,&GamePanel::disposCard);
    connect(user,&Player::noticyStoreCard,this,&GamePanel::disposCard);
}

void GamePanel::initCard()
{
    //加载大图
    QPixmap pixmap(":/images/card.png");
    //计算每张图大小
    m_cardSize.setWidth(pixmap.width() / 13);
    m_cardSize.setHeight(pixmap.height() / 5);
    //背景图
    m_cardBackImg = pixmap.copy(m_cardSize.width()*2,m_cardSize.height()*4,
                                m_cardSize.width(),m_cardSize.height());
    //正常花色
    for(int i = 0,suit=Card::Suit_Begin+1;suit<Card::Suit_End;suit++,i++)
    {
        for(int j = 0,point=Card::Card_Begin+1;point<Card::Card_SJ;point++,j++)
        {
            Card card((Card::CardPoint)point,(Card::CardSuit)suit);
            //裁剪图片
            cropImage(pixmap,m_cardSize.width()*j,m_cardSize.height()*i,card);
        }
    }
    //大小王
    Card c;
    c.setPoint(Card::Card_SJ);
    c.setSuit(Card::Suit_Begin);
    cropImage(pixmap,0,m_cardSize.height()*4,c);

    c.setPoint(Card::Card_BJ);
    c.setSuit(Card::Suit_Begin);
    cropImage(pixmap,m_cardSize.width(),m_cardSize.height()*4,c);
}

void GamePanel::cropImage(QPixmap &pix, int x, int y,Card& c)
{
    QPixmap pixmap = pix.copy(x,y,m_cardSize.width(),m_cardSize.height());
    CardPannel* pannel = new CardPannel(this);
    pannel->setPixmap(pixmap,m_cardBackImg);
    pannel->setCard(c);
    pannel->hide();
    m_cardMap.insert(c,pannel);
    //处理卡牌点击
    connect(pannel,&CardPannel::currSelectCard,this,&GamePanel::onCurrSelectCard);
}

void GamePanel::initButtonsGroup()
{
    ui->btnGroup->initButtons();
    ui->btnGroup->selectPannel(ButtonGroup::Pannel::Start);
    connect(ui->btnGroup,&ButtonGroup::start,this,[=](){
        //界面初始化
        ui->btnGroup->selectPannel(ButtonGroup::Empty);
        m_gameCtrol->clearPlayerScore();
        scoreUpdate();
        //修改游戏状态
        gameStatusPrecess(GameControl::DispatchCard);
        //播放背景音乐
        m_music->startBackMusic(80);
    });
    connect(ui->btnGroup,&ButtonGroup::playHand,this,&GamePanel::userPlayHand);
    connect(ui->btnGroup,&ButtonGroup::pass,this,&GamePanel::userPass);
    connect(ui->btnGroup,&ButtonGroup::betPoint,this,[=](int point){
        m_gameCtrol->getUserPlayer()->grabLordBet(point);
        ui->btnGroup->selectPannel(ButtonGroup::Empty);
    });
}

void GamePanel::initPlayerContext()
{
    // 1. 放置玩家扑克牌的区域
    const QRect cardsRect[] =
        {
            // x, y, width, height
            QRect(90, 130, 100, height() - 200),                    // 左侧机器人
            QRect(rect().right() - 190, 130, 100, height() - 200),  // 右侧机器人
            QRect(250, rect().bottom() - 120, width() - 500, 100)   // 当前玩家
        };
    // 2. 玩家出牌的区域
    const QRect playHandRect[] =
        {
            QRect(260, 150, 100, 100),                              // 左侧机器人
            QRect(rect().right() - 360, 150, 100, 100),             // 右侧机器人
            QRect(150, rect().bottom() - 290, width() - 300, 105)   // 当前玩家
        };
    // 3. 玩家头像显示的位置
    const QPoint roleImgPos[] =
        {
            QPoint(cardsRect[0].left()-80, cardsRect[0].height() / 2 + 20),     // 左侧机器人
            QPoint(cardsRect[1].right()+10, cardsRect[1].height() / 2 + 20),    // 右侧机器人
            QPoint(cardsRect[2].right()-10, cardsRect[2].top() - 10)            // 当前玩家
        };

    //循环
    int index = m_playerList.indexOf(m_gameCtrol->getUserPlayer());
    for(int i =0;i<m_playerList.size();i++)
    {
        PlayerContext context;
        context.align = i==index ? Horizontal: Vertical;
        context.isForntSide = i==index ? true : false;
        context.cardRect = cardsRect[i];
        context.playHandRect = playHandRect[i];
        //提示信息
        context.info = new QLabel(this);
        context.info->resize(160,98);
        context.info->hide();
        //显示到中心
        QRect rect = playHandRect[i];
        QPoint p(rect.left()+(rect.width()-context.info->width())/2,
                 rect.top()+(rect.height()-context.info->height())/2);
        context.info->move(p);
        //玩家头像
        context.roleImg = new QLabel(this);
        context.roleImg->resize(84,120);
        context.roleImg->hide();
        context.roleImg->move(roleImgPos[i]);

        m_contextMap.insert(m_playerList[i],context);
    }

}

void GamePanel::initGameScene()
{
    //发牌区的扑克牌
    m_baseCard = new CardPannel(this);
    m_baseCard->setPixmap(m_cardBackImg,m_cardBackImg);
    //移动的扑克牌
    m_moveCard = new CardPannel(this);
    m_moveCard->setPixmap(m_cardBackImg,m_cardBackImg);
    //三张底牌
    for(int i =0;i<3;i++)
    {
        CardPannel* pannel = new CardPannel(this);
        pannel->setPixmap(m_cardBackImg,m_cardBackImg);
        m_last3Cards.push_back(pannel);
    }
    //扑克牌的位置
    m_baseCardPoint = QPoint((width()-m_cardSize.width())/2,
                             height()/2-100);
    m_baseCard->move(m_baseCardPoint);
    m_moveCard->move(m_baseCardPoint);

    int baseX = (width()-(m_cardSize.width()*3+10*2))/2;
    for(int i = 0 ;i<3;i++)
    {
        m_last3Cards[i]->move(baseX+(m_cardSize.width()+10)*i,20);
        m_last3Cards[i]->hide();
    }
}

void GamePanel::gameStatusPrecess(GameControl::GameStatus status)
{
    m_status = status;
    switch(status)
    {
    case GameControl::DispatchCard:
        startDispatchCard();
        break;
    case GameControl::PlayingHand:
    {
        //隐藏发牌区域的底牌和移动的牌
        m_baseCard->hide();
        m_moveCard->hide();
        //显示三张底牌
        for(int i =0;i<m_last3Cards.size();i++)
        {
            m_last3Cards.at(i)->show();
        }
        for(int i = 0;i<m_playerList.size();++i)
        {
            PlayerContext &context = m_contextMap[m_playerList.at(i)];
            //隐藏抢地主时的提示信息
            context.info->hide();
            //显示人物头像，设置位置和朝向
            QPixmap pix = loadRoleImage(m_playerList.at(i)->getRole(),m_playerList.at(i)->getSex(),m_playerList.at(i)->getDirection());
            context.roleImg->setPixmap(pix);
            context.roleImg->show();
        }
        break;
    }
    case GameControl::CallingLord:
    {
        //取出底牌
        CardList last3Cards = m_gameCtrol->getSurplusCards().toCardList();
        //显示底牌
        for(int i = 0 ;i<last3Cards.size();++i)
        {
            QPixmap pix = m_cardMap[last3Cards[i]]->getPixmap();
            m_last3Cards[i]->setPixmap(pix,m_cardBackImg);
            m_last3Cards[i]->setCardSide(true);
            m_last3Cards[i]->hide();
        }
        //开始叫地主
        m_gameCtrol->startLordCall();
        break;
    }
    default:
        break;
    }
}

void GamePanel::startDispatchCard()
{
    //重置每张牌的属性
    for(auto it = m_cardMap.begin();it!=m_cardMap.end();++it)
    {
        it.value()->setSelected(false);
        it.value()->setCardSide(true);
        it.value()->hide();
    }
    //隐藏三张卡牌
    for(int i = 0;i<3;i++)
    {
        m_last3Cards.at(i)->hide();
    }
    //重置玩家窗口的上下文信息
    int index = m_playerList.indexOf(m_gameCtrol->getUserPlayer());
    for(int it = 0;it<m_contextMap.size();it++)
    {
        m_contextMap[m_playerList.at(it)].lastCards.clear();
        m_contextMap[m_playerList.at(it)].info->hide();
        m_contextMap[m_playerList.at(it)].isForntSide = it==index?true:false;
        m_contextMap[m_playerList.at(it)].roleImg->hide();
    }
    //重置所有玩家卡牌
    m_gameCtrol->resetCardData();
    //显示要发的牌
    m_baseCard->show();
    //隐藏按钮
    ui->btnGroup->selectPannel(ButtonGroup::Empty);
    //启动定时器
    m_time->start(10);
    //播放背景音乐
    m_music->playAssistMusic(MusicControl::AssistMusic::Dispatch);
}

void GamePanel::onDispatchCard()
{
    //记录扑克牌的位置
    static int curpos = 0;
    //当前玩家
    Player* play = m_gameCtrol->getCurrentPlyer();
    //判断是否发完一张牌
    if(curpos >= 100)
    {
        //给玩家发一张牌
        Card card = m_gameCtrol->takeOneCard();
        play->storeDispatchCard(card);
        //更新移动卡牌的窗口
        Cards c(card);
        // disposCard(play,c);      //已经做了信号槽，不用了
        //切换玩家
        m_gameCtrol->setCurrentPlayer(play->getNextPlayer());
        moveCard(play,curpos);
        curpos = 0;
        //判断是否发完除了三张底牌的牌
        if(m_gameCtrol->getSurplusCards().cardCount() == 3)
        {
            m_time->stop();
            //切换状态
            gameStatusPrecess(GameControl::CallingLord);
            //停止发牌音乐
            m_music->stopPlayAssistMusic();
            return;
        }
    }
    //移动扑克牌
    moveCard(play,curpos);
    curpos+=15;

}

void GamePanel::moveCard(Player *play, int pos)
{
    //得到玩家扑克牌的显示区域
    QRect cardRect = m_contextMap[play].cardRect;
    //每个玩家移动的步长
    const int num[] ={
        (m_baseCardPoint.x()-cardRect.left())/100,
        (cardRect.left()-m_baseCardPoint.x())/100,
        (cardRect.top()-m_baseCardPoint.y())/100,
    };
    //扑克牌的位置
    QPoint cardPoint[] ={
        QPoint(m_baseCardPoint.x()-num[0]*pos,m_baseCardPoint.y()),
        QPoint(m_baseCardPoint.x()+num[1]*pos,m_baseCardPoint.y()),
        QPoint(m_baseCardPoint.x(),m_baseCardPoint.y()+num[2]*pos),
    };
    //移动扑克牌
    int index = m_playerList.indexOf(play);
    m_moveCard->move(cardPoint[index]);

    //临界状态处理
    if(pos==0)
    {
        m_moveCard->show();
    }
    if(pos>=100)
    {
        m_moveCard->hide();
    }
}

void GamePanel::disposCard(Player *play,const Cards &cards)
{
    //得到传入的cards排序到vector容器中
    Cards constCards = const_cast<Cards&>(cards);
    CardList list = constCards.toCardList();
    for(int i = 0; i < list.size(); ++i)
    {
        CardPannel* pannel = m_cardMap[list.at(i)];
        pannel->cardOwner(play);
    }
    //更新扑克牌在窗口的显示
    updatePlayerCards(play);
}

void GamePanel::updatePlayerCards(Player *player)
{
    Cards cards = player->getCards();
    CardList list = cards.toCardList();

    m_rect = QRect();
    m_userCards.clear();

    //取出展示扑克牌的区域
    QRect rect= (m_contextMap[player].cardRect);
    int cardSpace = 20;
    //每次给牌会重新调用这个函数使玩家手里的牌降序
    for(int i = 0;i<list.size();i++)
    {
        CardPannel *pannel = m_cardMap[list.at(i)];
        pannel->show();
        pannel->raise(); //将此部件提升到父部件堆栈的顶部。调用此函数后，该部件在视觉上会位于任何重叠的兄弟部件的前面。
        pannel->setCardSide(m_contextMap[player].isForntSide);

        //水平还是垂直
        if(m_contextMap[player].align == Horizontal)
        {
            int leftX = rect.left()+(rect.width()-(list.size()-1)*cardSpace-m_cardSize.width())/2;
            int topY = rect.top()+(rect.height()-m_cardSize.height())/2;
            if(pannel->isSelected())
            {
                topY-=10;
            }
            pannel->move(leftX+i*cardSpace,topY);
            //所有卡牌所在区域
            m_rect = QRect(leftX,topY,pannel->width()+i*cardSpace,pannel->height());
            int curWidth = 0;
            if(i == list.size()-1)  //最后一张牌是完整的
            {
                curWidth = pannel->width();
            }else
            {
                curWidth = cardSpace;
            }
            //单张卡牌可点区域
            QRect cardRect(leftX+i*cardSpace,topY,curWidth,pannel->height());
            m_userCards.insert(pannel,cardRect);

        }else
        {
            int leftX = rect.left()+(rect.width()-m_cardSize.width())/2;
            int topY = rect.top()+(rect.height()-(list.size()-1)*cardSpace-m_cardSize.height())/2;
            pannel->move(leftX,topY+i*cardSpace);
        }
    }

    //显示出牌区域的牌
    //得到出牌区域
    QRect playHandRect= m_contextMap[player].playHandRect;
    Cards playCards = m_contextMap[player].lastCards;
    //如果上次出牌不为空，有出牌，
    if(!playCards.isEmpty())
    {
        int playSpace = 24;
        CardList clist = playCards.toCardList();
        int i = 0;
        for(auto it = clist.begin(); it!=clist.end(); it++,i++)
        {
            CardPannel* pannel = m_cardMap[*it];
            pannel->setCardSide(true);
            pannel->raise();
            //将牌移动到出牌区域
            if(m_contextMap[player].align == Horizontal)
            {
                int leftX = playHandRect.left()+(playHandRect.width()-(clist.size()-1)*playSpace-pannel->width())/2;
                int topY = playHandRect.top()+(playHandRect.height()-pannel->height())/2;
                pannel->move(leftX+i*playSpace,topY);
            }else
            {
                int leftX = playHandRect.left()+(playHandRect.width()-pannel->width())/2;
                int topY = playHandRect.top();  //防止机器人出牌区域与玩家出牌区域重叠
                pannel->move(leftX,topY+i*playSpace);
            }
            pannel->show();
        }
    }

}

QPixmap GamePanel::loadRoleImage(Player::Role role, Player::Sex sex, Player::Direction direction)
{
    QVector<QString> lordMan;
    QVector<QString> lordWoman;
    QVector<QString> farmerMan;
    QVector<QString> farmerWoman;
    lordMan << ":/images/lord_man_1.png" << ":/images/lord_man_2.png";
    lordWoman << ":/images/lord_woman_1.png" << ":/images/lord_woman_2.png";
    farmerMan << ":/images/farmer_man_1.png" << ":/images/farmer_man_2.png";
    farmerWoman << ":/images/farmer_woman_1.png" << ":/images/farmer_woman_2.png";

    //因为图片头像朝向都朝右，所以使用QImage
    QImage image;
    int rand = QRandomGenerator::global()->bounded(2);
    if(role == Player::Lord && sex == Player::Man)
    {
        image.load(lordMan.at(rand));
    }else if(role == Player::Lord && sex == Player::Woman)
    {
        image.load(lordWoman.at(rand));
    }else if(role == Player::Farmer && sex == Player::Man)
    {
        image.load(farmerMan.at(rand));
    }else if(role == Player::Farmer && sex == Player::Woman)
    {
        image.load(farmerWoman.at(rand));
    }

    QPixmap pix;
    if(direction == Player::Left)
    {
        //左侧不需要镜像
        pix = QPixmap::fromImage(image);
    }else
    {
        pix = QPixmap::fromImage(image.mirrored(true,false));
    }
    return pix;
}

void GamePanel::onPlayerStatusChanged(Player *player, GameControl::PlayerStatus status)
{
    switch (status) {
    case GameControl::ThinkingForCallLord:
        //玩家就显示按钮组
        //机器人没有处理，有prepareCallLord线程处理
        if(player == m_gameCtrol->getUserPlayer())
        {
            ui->btnGroup->selectPannel(ButtonGroup::CallLord,m_gameCtrol->getMaxBet());
        }
        break;
    case GameControl::ThinkingForPlayHand:
        //1.隐藏上一轮打出的牌
        hidePlayerCards(player);
        if(player == m_gameCtrol->getUserPlayer())
        {
            //判断是不是必须出牌（出牌玩家）
            if(player == m_gameCtrol->getPendPlayer() || m_gameCtrol->getPendPlayer() == nullptr)
            {
                ui->btnGroup->selectPannel(ButtonGroup::PlayCard);
            }else
            {
                ui->btnGroup->selectPannel(ButtonGroup::PlayOrPass);
            }
        }else
        {
            ui->btnGroup->selectPannel(ButtonGroup::Empty);
        }
        break;
    case GameControl::Winning:
        //停止背景音乐
        m_music->stopBackMusic();

        m_contextMap[m_gameCtrol->getLeftRobot()].isForntSide = true;
        m_contextMap[m_gameCtrol->getRightRobot()].isForntSide = true;
        updatePlayerCards(m_gameCtrol->getLeftRobot());
        updatePlayerCards(m_gameCtrol->getRightRobot());
        //更新玩家得分
        scoreUpdate();
        //设置当前玩家
        m_gameCtrol->setCurrentPlayer(player);
        showEndingPanel();
        break;
    default:
        break;
    }
}

void GamePanel::onGrapLord(Player *player, int point, bool flag)
{
    //显示抢地主的信息提示
    PlayerContext context = m_contextMap[player];
    if(point == 0)
    {
        context.info->setPixmap(QPixmap(":/images/buqinag.png"));
    }else
    {
        if(flag)
        {
            context.info->setPixmap(QPixmap(":/images/jiaodizhu.png"));
        }else
        {
            context.info->setPixmap(QPixmap(":/images/qiangdizhu.png"));
        }
        //显示下注分数
        showAnimation(AnimationType::Bet,point);
    }
    context.info->show();

    //播放背景音乐
    m_music->playRobLord(point,(MusicControl::RoleSex)player->getSex(),flag);
}

void GamePanel::onPlayHand(Player *player,const Cards &card)
{
    Cards& cards = const_cast<Cards&>(card);
    //存储打出的牌
    auto it = m_contextMap.find(player);
    it->lastCards = cards;
    //1.播放牌型播放游戏音效
    PlayHand hand(cards);
    PlayHand::HandType type = hand.getType();
    if(type == PlayHand::Hand_Plane || type == PlayHand::Hand_Plane_signal || type == PlayHand::Hand_Plane_Pair)
    {
        showAnimation(AnimationType::FeiJi);
    }else if(type == PlayHand::Hand_Seq_Signal)
    {
        showAnimation(AnimationType::ShunZi);
    }else if(type == PlayHand::Hand_Seq_Pair)
    {
        showAnimation(AnimationType::LianDui);
    }else if(type == PlayHand::Hand_Bomb)
    {
        showAnimation(AnimationType::Bomb);
    }else if(type == PlayHand::Hand_Bomb_Jokers)
    {
        showAnimation(AnimationType::JokerBomb);
    }
    //玩家打出的牌是不是空牌（不出）
    if(cards.isEmpty())
    {
        it->info->setPixmap(QPixmap(":/images/pass.png"));
        it->info->show();
        m_music->playPassMusic((MusicControl::RoleSex)player->getSex());
    }else
    {
        //3.播放提示音乐
        if(m_gameCtrol->getPendPlayer() == player || m_gameCtrol->getPendPlayer() == nullptr)
        {
            m_music->playCardsMusic(cards,true,(MusicControl::RoleSex)player->getSex());
        }
        else
        {
            m_music->playCardsMusic(cards,false,(MusicControl::RoleSex)player->getSex());
        }
    }
    //2.更新玩家手里的牌
    updatePlayerCards(player);
    //判断玩家手里的牌的数量
    if(!cards.isEmpty() && player->getCards().cardCount() == 2)
    {
        m_music->playLastMusic(MusicControl::Last2,(MusicControl::RoleSex)player->getSex());
    }else if(!cards.isEmpty() && player->getCards().cardCount() == 1)
    {
        m_music->playLastMusic(MusicControl::Last1,(MusicControl::RoleSex)player->getSex());
    }
}

void GamePanel::onCurrSelectCard(Qt::MouseButton button)
{
    //判断是不是出牌状态
    if(m_status == GameControl::CallingLord || m_status == GameControl::DispatchCard) return;
    //判断卡牌拥有者是不是用户玩家
    CardPannel* pannel = static_cast<CardPannel*>(sender());  //sender:获取信号发送者对象
    if(pannel->getOwner() != m_gameCtrol->getUserPlayer()) return;
    //保存当前卡牌窗口对象
    m_currSelectedCard = pannel;
    //鼠标左键还是右键
    if(button == Qt::LeftButton)
    {
        //设置扑克牌的选中状态
        pannel->setSelected(!pannel->isSelected());
        //更新扑克牌在窗口中的显示
        updatePlayerCards(pannel->getOwner());
        //保存或删除扑克牌的窗口对象
        QSet<CardPannel*>::iterator it = m_currSelectCards.find(pannel);
        //集合里面有没有该窗口对象
        if(it!=m_currSelectCards.end())
        {
            //有就删除（第二次点击）
            m_currSelectCards.remove(*it);
        }else
        {
            m_currSelectCards.insert(pannel);
        }
        m_music->playAssistMusic(MusicControl::AssistMusic::SelectCard);
    }else if(button == Qt::RightButton)
    {
        //调用出牌按钮的槽函数
        userPlayHand();
    }
}

void GamePanel::userPlayHand()
{
    //判断是不是出牌阶段
    if(m_status != GameControl::PlayingHand) return;
    //判断是不是用户玩家
    if(m_gameCtrol->getCurrentPlyer() != m_gameCtrol->getUserPlayer()) return;
    //判断要出的牌是不是为空
    if(m_currSelectCards.isEmpty()) return;
    //判断要出的牌的牌型
    Cards cards;
    for(auto it = m_currSelectCards.begin(); it!=m_currSelectCards.end(); ++it)
    {
        Card card = (*it)->getCard();
        cards.add(card);
    }
    PlayHand hand(cards);
    PlayHand::HandType type = hand.getType();
    if(type == PlayHand::Hand_UnKonw) return;
    //判断是压牌还是随便出牌
    if(m_gameCtrol->getPendPlayer() != m_gameCtrol->getUserPlayer())
    {
        Cards c = m_gameCtrol->getPendCards();
        if(!hand.canBeat(PlayHand(c))) return;
    }
    //通过玩家对象出牌
    m_gameCtrol->getUserPlayer()->playHand(cards);

    m_clock->stopTimer();
    //清空容器
    m_currSelectCards.clear();
}

void GamePanel::userPass()
{
    m_clock->stopTimer();
    //判断是不是用户玩家
    if(m_gameCtrol->getCurrentPlyer() != m_gameCtrol->getUserPlayer()) return;
    //打出一个空的Cards对象
    Cards empty;
    m_gameCtrol->getUserPlayer()->playHand(empty);
    //清空容器（让选中的卡牌归位）
    for(auto it = m_currSelectCards.begin();it!=m_currSelectCards.end();++it)
    {
        (*it)->setSelected(false);
    }
    m_currSelectCards.clear();
    //更新卡牌窗口
    updatePlayerCards(m_gameCtrol->getUserPlayer());
}

void GamePanel::showEndingPanel()
{
    bool isLord = m_gameCtrol->getUserPlayer()->getRole() == Player::Lord ? true:false;
    bool isWin = m_gameCtrol->getUserPlayer()->isWin();
    EndingPanel* panel = new EndingPanel(isLord,isWin,this);
    panel->move((width()-panel->width())/2,-panel->height());
    panel->show();
    panel->setPlayerScore(m_gameCtrol->getLeftRobot()->getSocre(),
                          m_gameCtrol->getRightRobot()->getSocre(),
                          m_gameCtrol->getUserPlayer()->getSocre());

    m_music->playEndingMusic(isWin);

    //设置动画效果
    QPropertyAnimation* animation = new QPropertyAnimation(panel,"geometry",this);
    //持续时间
    animation->setDuration(1500);
    //起止位置
    animation->setStartValue(QRect(panel->x(),panel->y(),panel->width(),panel->height()));
    animation->setEndValue(QRect((width()-panel->width())/2,(height()-panel->height())/2,panel->width(),panel->height()));
    //设置曲线
    animation->setEasingCurve(QEasingCurve(QEasingCurve::OutBounce));
    //开始动画
    animation->start();

    //处理信号槽
    connect(panel,&EndingPanel::gameContinue,this,[=](){
        panel->close();
        animation->deleteLater();
        panel->deleteLater();
        ui->btnGroup->selectPannel(ButtonGroup::Empty);
        gameStatusPrecess(GameControl::DispatchCard);
        //播放背景音乐
        m_music->startBackMusic(80);
    });
}

void GamePanel::initAlarmClock()
{
    m_clock = new AlarmClock(this);
    m_clock->move((width()-m_clock->width())/2,(height()-m_clock->height())/2+30);
    connect(m_clock,&AlarmClock::notMuchTime,this,[=](){
        //播放提示音乐
        m_music->playAssistMusic(MusicControl::AssistMusic::Alert);
    });
    connect(m_clock,&AlarmClock::timeOut,this,&GamePanel::userPass);
    UserPlayer* userPlayer = m_gameCtrol->getUserPlayer();
    connect(userPlayer,&UserPlayer::startClock,this,[=](){
        //不是第一个出牌的才有时间限制
        if(m_gameCtrol->getPendPlayer()!=userPlayer && m_gameCtrol->getPendPlayer() != nullptr)
        {
            m_clock->startTime();
        }
    });
}

void GamePanel::hidePlayerCards(Player *player)
{
    auto it = m_contextMap.find(player);
    //判断是不是找到了
    if(it != m_contextMap.end())
    {
        //判断有没有出牌
        if(it->lastCards.isEmpty())
        {
            //没有出就会有“不出”的提示信息-->隐藏
            it->info->hide();
        }else
        {
            //Cards --> Card
            CardList list = it->lastCards.toCardList();
            for(auto it2 = list.begin();it2!=list.end();++it2)
            {
                //隐藏卡牌窗口
                m_cardMap[*it2]->hide();
            }
        }
        it->lastCards.clear();
    }
}

void GamePanel::showAnimation(AnimationType type, int bet)
{
    switch (type) {
    case AnimationType::ShunZi:
    case AnimationType::LianDui:
        m_animation->setFixedSize(250,150);
        m_animation->move((width()-m_animation->width())/2,200);
        m_animation->showSeqSignelOrPair((AnimationWindow::Type)type);
        break;
    case AnimationType::FeiJi:
        m_animation->setFixedSize(800,75);
        m_animation->move((width()-m_animation->width())/2,200);
        m_animation->showPlane();
        break;
    case AnimationType::Bomb:
        m_animation->setFixedSize(180,200);
        m_animation->move((width()-m_animation->width())/2,(height()-m_animation->height())/2-70);
        m_animation->showBombOrJokerBomb(AnimationWindow::Bomb);
        break;
    case AnimationType::JokerBomb:
        m_animation->setFixedSize(250,200);
        m_animation->move((width()-m_animation->width())/2,(height()-m_animation->height())/2-70);
        m_animation->showBombOrJokerBomb(AnimationWindow::JokerBomb);
        break;
    case AnimationType::Bet:
        m_animation->setFixedSize(160,98);
        m_animation->move((width()-m_animation->width())/2,(height()-m_animation->height())/2-140);
        m_animation->showBetScore(bet);
        break;
    default:
        break;
    }
    m_animation->show();
}

void GamePanel::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(),m_bkPix);
}

void GamePanel::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons() & Qt::LeftButton)
    {
        QPoint pt = ev->pos();
        //有没有点击到卡牌
        if(!m_rect.contains(pt))
        {
            m_currSelectedCard = nullptr;
        }else
        {
            QList<CardPannel*> list = m_userCards.keys();
            for(int i=0 ; i<list.size() ;++i)
            {
                CardPannel* pannel = list.at(i);
                //拖动点在卡牌区域&&
                if(m_userCards[pannel].contains(pt) && m_currSelectedCard != pannel)
                {
                    //点击卡牌
                    pannel->click();
                    // m_currSelectedCard = pannel;
                }
            }
        }
    }
}
