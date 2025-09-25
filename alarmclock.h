#ifndef ALARMCLOCK_H
#define ALARMCLOCK_H

#include <QTimer>
#include <QWidget>

class AlarmClock : public QWidget
{
    Q_OBJECT
public:
    explicit AlarmClock(QWidget *parent = nullptr);

    void startTime();
    void stopTimer();

signals:
    void notMuchTime();
    void timeOut();
protected:
    void paintEvent(QPaintEvent* ev);
private:
    QPixmap m_clock;
    QPixmap m_num;
    int m_timeChanged;
    QTimer* m_time;
};

#endif // ALARMCLOCK_H
