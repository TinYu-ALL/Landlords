#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QEvent>
#include <QPushButton>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = nullptr);

    void setImage(QString normal,QString pressed,QString hover);

protected:
    //鼠标按下
    void mousePressEvent(QMouseEvent *ev);
    //鼠标释放
    void mouseReleaseEvent(QMouseEvent* ev);
    //鼠标进入
    void enterEvent(QEvent* ev);
    //鼠标离开
    void leaveEvent(QEvent* ev);
    //绘图
    void paintEvent(QPaintEvent* ev);

signals:
private:
    QString m_normal;
    QString m_pressed;
    QString m_hover;
    QPixmap m_pix;

};

#endif // MYBUTTON_H
