#ifndef ROBOTGRAPLORD_H
#define ROBOTGRAPLORD_H

#include "robot.h"

#include <QObject>
#include <QThread>

class RobotGraplord : public QObject
{
    Q_OBJECT
public:
    explicit RobotGraplord(Robot* robot,QObject *parent = nullptr);

    void work();

    // void run()override;

signals:
    void finish();
private:
    Robot* m_player;
};

#endif // ROBOTGRAPLORD_H
