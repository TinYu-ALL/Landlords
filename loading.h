#ifndef LOADING_H
#define LOADING_H

#include <QWidget>

class Loading : public QWidget
{
    Q_OBJECT
public:
    explicit Loading(QWidget *parent = nullptr);

signals:
protected:
    void paintEvent(QPaintEvent* ev);

private:
    QPixmap m_backGroung;
    QPixmap m_progress;
    int step = 0;
};

#endif // LOADING_H
