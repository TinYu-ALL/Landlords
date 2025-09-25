#ifndef ROBOTPLAYHAND_H
#define ROBOTPLAYHAND_H

#include <QObject>
#include "cards.h"

#include "player.h"
class RobotPlayHand : public QObject
{
    Q_OBJECT
public:
    explicit RobotPlayHand(Player* robot,QObject *parent = nullptr);

    void work();

    // void run()override;

signals:
    void strategyReady(const Cards& cards);
private:
    Player* m_player;
    Cards m_cards;
};

#endif // ROBOTPLAYHAND_H
