#include "mybutton.h"

#include <QMouseEvent>
#include <QPainter>

MyButton::MyButton(QWidget *parent)
    : QPushButton{parent}
{}

void MyButton::setImage(QString normal, QString pressed, QString hover)
{
    m_normal = normal;
    m_pressed = pressed;
    m_hover = hover;
    m_pix.load(m_normal);
    update();
}

void MyButton::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        m_pix.load(m_pressed);
        update();
    }
    QPushButton::mousePressEvent(ev);
}

void MyButton::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        m_pix.load(m_normal);
        update();
    }
    QPushButton::mouseReleaseEvent(ev);
}

void MyButton::enterEvent(QEvent *)
{
    m_pix.load(m_hover);
    update();
}

void MyButton::leaveEvent(QEvent *)
{
    m_pix.load(m_normal);
    update();
}

void MyButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(rect(),m_pix);
}
