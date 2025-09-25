#include "loading.h"

#include "gamepannel.h"

#include <QPainter>
#include <QTimer>

Loading::Loading(QWidget *parent)
    : QWidget{parent}
{
    m_backGroung.load(":/images/loading.png");
    setFixedSize(m_backGroung.size());

    //去除边框 不显示标题栏、边框、系统按钮 | 当前窗口已有的 flags
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //背景透明
    setAttribute(Qt::WA_TranslucentBackground);

    QPixmap pix = QPixmap(":/images/progress.png");
    QTimer* timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        m_progress = pix.copy(0,0,step,pix.height());
        update();
        if(step >= pix.width())
        {
            timer->stop();
            timer->deleteLater();
            GamePanel* panel = new GamePanel;
            panel->show();
            close();
        }
        step += 10;
    });
    timer->start(15);
}

void Loading::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(),m_backGroung);
    p.drawPixmap(62,417,m_progress.width(),m_progress.height(),m_progress);
}
