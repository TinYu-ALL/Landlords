#ifndef ENDINGPANEL_H
#define ENDINGPANEL_H

#include "qlabel.h"
#include "scorepannel.h"
#include <QPushButton>
#include <QWidget>

class EndingPanel : public QWidget
{
    Q_OBJECT
public:
    explicit EndingPanel(bool isLord,bool isWin,QWidget *parent = nullptr);

    void setPlayerScore(int left,int right,int self);

signals:
    void gameContinue();

protected:
    void paintEvent(QPaintEvent* ev);
private:
    QPixmap m_pix;
    QLabel* m_label;
    ScorePannel* m_score;
    QPushButton* m_button;
};

#endif // ENDINGPANEL_H
