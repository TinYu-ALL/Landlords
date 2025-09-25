#include "animationwindow.h"

#include <QPainter>
#include <QTimer>

AnimationWindow::AnimationWindow(QWidget *parent)
    : QWidget{parent}
{m_x = 0;m_flag = 0;}

void AnimationWindow::showBetScore(int bet)
{
    m_x= 0;

    if(bet == 1)
    {
        m_pix.load(":/images/score1.png");
    }else if(bet == 2)
    {
        m_pix.load(":/images/score2.png");
    }else if(bet == 3)
    {
        m_pix.load(":/images/score3.png");
    }
    update();
    QTimer::singleShot(2000,this,&AnimationWindow::hide);
}

void AnimationWindow::showSeqSignelOrPair(Type type)
{
    m_x= 0;

    QString name = type == signel ? ":/images/shunzi.png" : ":/images/liandui.png";
    m_pix.load(name);
    update();
    QTimer::singleShot(2000,this,&AnimationWindow::hide);
}

void AnimationWindow::showBombOrJokerBomb(BombType type)
{
    m_x= 0;

    m_flag = 0;
    int flag = type == Bomb ? 12 : 8;
    QString name = type == Bomb ? "bomb" : "joker_bomb";
    QTimer* timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        m_flag++;
        if(m_flag > flag)
        {
            timer->stop();
            timer->deleteLater();
            hide();
            return;
        }
        QString path = QString(":/images/%1_%2.png").arg(name).arg(m_flag);
        m_pix.load(path);
        update();
    });
    timer->start(60);
}


void AnimationWindow::showPlane()
{
    m_x = width();
    m_pix.load(":/images/plane_1.png");
    setFixedHeight(m_pix.height());
    update();

    //窗口分为5部分，不同时间放不同的图片实现动态效果
    int step = width() / 5;
    QTimer* timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        static int length = 0; //用来判断走过了窗口的一部分没有
        static int name = 0; //用来选择1-5的图片
        length += 5;
        if(length >= step)
        {
            length = 0;
            name++;
            QString path = QString(":/images/plane_%1.png").arg(name % 5 + 1);
            m_pix.load(path);
        }
        if(m_x <= -110)
        {
            timer->stop();
            timer->deleteLater();
            length = name = 0;
            hide();
            return;
        }
        m_x -= 5;
        update();
    });
    timer->start(15);
}

void AnimationWindow::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(m_x,0,m_pix.width(),m_pix.height(),m_pix);
}
