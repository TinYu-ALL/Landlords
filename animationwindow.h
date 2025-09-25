#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QWidget>

class AnimationWindow : public QWidget
{
    Q_OBJECT
public:
    enum Type{signel,pair};
    enum BombType{Bomb,JokerBomb};
    explicit AnimationWindow(QWidget *parent = nullptr);

    //展示分数窗口
    void showBetScore(int bet);
    //显示顺子或连对
    void showSeqSignelOrPair(Type type);
    //显示王炸或王炸
    void showBombOrJokerBomb(BombType type);
    //显示飞机
    void showPlane();

protected:
    void paintEvent(QPaintEvent* ev);

private:
    QPixmap m_pix;
    int m_flag;
    int m_x;

signals:
};

#endif // ANIMATIONWINDOW_H
