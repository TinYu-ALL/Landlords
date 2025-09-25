#include "robotgraplord.h"

#include <QTimer>

RobotGraplord::RobotGraplord(Robot *robot, QObject *parent)
    : QObject{parent}
{
    m_player = robot;
}

void RobotGraplord::work()
{
    QTimer::singleShot(2000,[=](){
        m_player->thinkCallLord();
        emit finish();
    });
}

// void RobotGraplord::run()
// {
//     msleep(1000);
//     m_player->thinkCallLord();
// }

