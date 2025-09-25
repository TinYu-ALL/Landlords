#include "endingpanel.h"

#include <QPainter>

EndingPanel::EndingPanel(bool isLord, bool isWin, QWidget *parent)
    : QWidget{parent}
{
    m_pix.load(":/images/gameover.png");
    setFixedSize(m_pix.size());

    //显示游戏状态
    m_label = new QLabel(this);
    if(isLord && isWin)
    {
        m_label->setPixmap(QPixmap(":/images/lord_win.png"));
    }else if(isLord && !isWin)
    {
        m_label->setPixmap(QPixmap(":/images/lord_fail.png"));
    }else if(!isLord && isWin)
    {
        m_label->setPixmap(QPixmap(":/images/farmer_win.png"));
    }else if(!isLord && !isWin)
    {
        m_label->setPixmap(QPixmap(":/images/farmer_fail.png"));
    }
    m_label->move(125,125);

    //显示分数面板
    m_score = new ScorePannel(this);
    m_score ->move(75,230);
    m_score->setFixedSize(270,160);
    m_score->setMyFont(14);
    m_score->setMyColor(ScorePannel::green);

    //继续按钮
    m_button = new QPushButton(this);
    m_button->move(84,429);
    m_button->setFixedSize(231,48);
    QString style = R"(
        QPushButton{border-image : url(:/images/button_normal.png)}
        QPushButton:hover{border-image : url(:/images/button_hover.png)}
        QPushButton:pressed{border-image : url(:/images/button_pressed.png)}
    )";
    m_button->setStyleSheet(style);
    connect(m_button,&QPushButton::clicked,this,&EndingPanel::gameContinue);
}

void EndingPanel::setPlayerScore( int left, int right,int self)
{
    m_score->setScore(left,right,self);
}

void EndingPanel::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(),m_pix);
}
