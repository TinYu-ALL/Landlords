#include "alarmclock.h"

#include <QPainter>

AlarmClock::AlarmClock(QWidget *parent)
    : QWidget{parent}
{
    m_time = new QTimer(this);
    setFixedSize(70,70);
    connect(m_time,&QTimer::timeout,this,[=](){
        m_timeChanged--;
        if(m_timeChanged<10 && m_timeChanged>0)
        {
            m_clock.load(":/images/clock.png");
            m_num=QPixmap(":/images/number.png").copy(m_timeChanged*(30+10),0,30,42).scaled(20,30);
            if(m_timeChanged == 5)
            {
                emit notMuchTime();
            }

        }else if(m_timeChanged<=0)
        {
            m_time->stop();
            m_clock = QPixmap();
            m_num = QPixmap();
            emit timeOut();
        }
        update();
    });
}

void AlarmClock::startTime()
{
    m_timeChanged = 15;
    m_time->start(1000);
}

void AlarmClock::stopTimer()
{
    m_time->stop();
    m_clock = QPixmap();
    m_num = QPixmap();
    update();
}

void AlarmClock::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(),m_clock);
    p.drawPixmap(24,24,m_num.width(),m_num.height(),m_num);
}
