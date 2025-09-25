#include "musiccontrol.h"

#include "playhand.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QTimer>

MusicControl::MusicControl(QObject *parent)
    : QObject{parent}
{
    for(int i=0;i<5;++i)
    {
        QMediaPlayer* player = new QMediaPlayer(this);
        QMediaPlaylist* list = new QMediaPlaylist(this);
        if(i<2 || i == 4)
        {
            list->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        }
        else if(i==2)
        {
            list->setPlaybackMode(QMediaPlaylist::Loop);
        }
        player->setPlaylist(list);
        player->setVolume(100);
        m_players.push_back(player);
        m_lists.push_back(list);
    }
    initPlayList();
}

void MusicControl::initPlayList()
{
    QStringList list;
    list << "Man" << "Woman" << "BGM" << "Other" << "Ending";

    //读配置文件
    QFile file(":/conf/playList.json");
    bool flag = file.open(QFile::ReadOnly);
    if(flag == false) return;
    QByteArray json = file.readAll();
    file.close();

    //解析json数据
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();
    for(int i = 0; i < list.size(); ++i)
    {
        QJsonArray array = obj.value(list.at(i)).toArray();
        for(int j = 0;j < array.size(); ++j)
        {
            //初始化播放列表
            m_lists.at(i)->addMedia(QMediaContent(QUrl(array.at(j).toString())));
        }
    }
}

void MusicControl::startBackMusic(int volume)
{
    m_lists[2]->setCurrentIndex(0);
    m_players[2]->setVolume(volume);
    m_players[2]->play();
}

void MusicControl::stopBackMusic()
{
    m_players[2]->stop();
}

void MusicControl::playRobLord(int point, RoleSex sex, bool isFirst)
{
    int index =  sex == RoleSex::Man ? 0 : 1;
    if(isFirst && point > 0)
    {
        m_lists[index]->setCurrentIndex(CardType::Order);
    }else if(point == 0)
    {
        if(isFirst)
        {
            m_lists[index]->setCurrentIndex(CardType::NoOrder);
        }else
        {
            m_lists[index]->setCurrentIndex(CardType::NoRob);
        }
    }else if(point == 2)
    {
        m_lists[index]->setCurrentIndex(CardType::Rob1);
    }else if(point == 3)
    {
        m_lists[index]->setCurrentIndex(CardType::Rob2);
    }
    m_players[index]->play();
}

void MusicControl::playCardsMusic(Cards cards, bool isFirst, RoleSex sex)
{
    int index =  sex == RoleSex::Man ? 0 : 1;
    QMediaPlaylist* list = m_lists[index];

    Card::CardPoint pt = Card::Card_Begin;
    PlayHand hand(cards);
    PlayHand::HandType type = hand.getType();
    if(type == PlayHand::Hand_Signal || type == PlayHand::Hand_Pair || type == PlayHand::Hand_Three)
    {
        pt = cards.takeRandomCard().Point();
    }
    int number = 0;
    switch(type)
    {
    // 单牌
    case PlayHand::Hand_Signal:
        number = pt - 1;    //card有一个card_begin，点数下标是从1开始的
        break;
    // 对牌
    case PlayHand::Hand_Pair:
        number = pt - 1 + 15;
        break;
    // 三张点数相同的牌
    case PlayHand::Hand_Three:
        number = pt - 1 + 15 + 13;
        break;
    // 三带一
    case PlayHand::Hand_Three_One:
        number = ThreeBindOne;
        break;
    // 三带二
    case PlayHand::Hand_Three_Pair:
        number = ThreeBindPair;
        break;
    // 飞机
    case PlayHand::Hand_Plane:
    // 飞机带两个单
    case PlayHand::Hand_Plane_signal:
    // 飞机带两个对儿
    case PlayHand::Hand_Plane_Pair:
        number = Plane;

        break;
    // 连对
    case PlayHand::Hand_Seq_Pair:
        number = SequencePair;
        break;
    // 顺子
    case PlayHand::Hand_Seq_Signal:
        number = Sequence;
        break;
    // 炸弹
    case PlayHand::Hand_Bomb:
        number = Bomb;
        break;
    // 王炸
    case PlayHand::Hand_Bomb_Jokers:
        number = JokerBomb;
        break;
    // 炸弹带一对儿
    case PlayHand::Hand_Bomb_Pair:
    // 炸弹带两单
    case PlayHand::Hand_Bomb_Two_Signal:
    // 王炸带一对儿
    case PlayHand::Hand_Bomb_Jokers_Pair:
    // 王炸带两单
    case PlayHand::Hand_Bomb_Jokers_Two_Signal:
        number = FourBindTwo;       //没有对应音乐所以播放四带2
        break;
    default:
        break;
    }

    if(isFirst == false && number >= CardType::Plane && number <= CardType::FourBindTwo)
    {
        //从大你，管住随机选一个
        list->setCurrentIndex(MoreBiger1 + QRandomGenerator::global()->bounded(2));
    }
    list->setCurrentIndex(number);
    m_players[index]->play();
    if(number == Bomb || number == JokerBomb)
    {
        playAssistMusic(BombVoice);
    }else if(number == Plane)
    {
        playAssistMusic(PlaneVoice);
    }
}

void MusicControl::playLastMusic(CardType type, RoleSex sex)
{
    int index = sex == Man ? 0 : 1;
    QMediaPlaylist* list = m_lists[index];
    //判断多媒体在不在播放音乐
    if(m_players[index]->state() == QMediaPlayer::StoppedState)
    {
        list->setCurrentIndex(type);
        m_players[index]->play();
    }else
    {
        QTimer::singleShot(1500,this,[=](){
            list->setCurrentIndex(type);
            m_players[index]->play();
        });
    }
}

void MusicControl::playPassMusic(RoleSex sex)
{
    int index = sex == Man ? 0 : 1;
    QMediaPlaylist* list = m_lists[index];
    int random = QRandomGenerator::global()->bounded(4);
    list->setCurrentIndex(Pass1 + random);
    m_players[index]->play();
}

void MusicControl::playAssistMusic(AssistMusic music)
{
    QMediaPlaylist::PlaybackMode mode;
    if(music == Dispatch)
    {
        //循环播放
        mode = QMediaPlaylist::CurrentItemInLoop;
    }else
    {
        //单曲一次
        mode = QMediaPlaylist::CurrentItemOnce;
    }
    QMediaPlaylist* list = m_lists[3];
    list->setCurrentIndex(music);
    list->setPlaybackMode(mode);
    m_players[3]->play();
}

void MusicControl::stopPlayAssistMusic()
{
    m_players[3]->stop();
}

void MusicControl::playEndingMusic(bool isWin)
{
    if(isWin)
    {
        m_lists[4]->setCurrentIndex(0);
    }else
    {
        m_lists[4]->setCurrentIndex(1);
    }
    m_players[4]->play();
}
