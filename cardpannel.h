#ifndef CARDPANNEL_H
#define CARDPANNEL_H

#include <Card.h>
#include <Player.h>
#include <QPaintEvent>
#include <QWidget>

class CardPannel : public QWidget
{
    Q_OBJECT
public:
    explicit CardPannel(QWidget *parent = nullptr);

    //设置、获取图片
    void setPixmap(const QPixmap &fornt,const QPixmap &black);
    QPixmap getPixmap();

    //扑克牌是哪一面
    void setCardSide(bool flag);
    bool isForntSide();

    //扑克牌是否被选中
    void setSelected(bool flag);
    bool isSelected();

    //扑克牌的花色和点数
    void setCard(const Card& card);
    Card getCard();

    //扑克牌的拥有者
    void cardOwner(Player* player);
    Player* getOwner();

    //模拟扑克牌的点击
    void click();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void currSelectCard(Qt::MouseButton button);
private:
    QPixmap m_fornt;
    QPixmap m_black;
    bool m_isfront = true;
    bool m_isSelect = false;
    Card m_card;
    Player* m_player = nullptr;

};

#endif // CARDPANNEL_H
