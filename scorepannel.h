#ifndef SCOREPANNEL_H
#define SCOREPANNEL_H

#include <QLabel>
#include <QWidget>

namespace Ui {
class ScorePannel;
}

class ScorePannel : public QWidget
{
    Q_OBJECT

public:
    enum FontColor{black,white,blue,red,green};
    explicit ScorePannel(QWidget *parent = nullptr);
    ~ScorePannel();

    //设置分数
    void setScore(int left,int right,int user);
    //设置字体
    void setMyFont(int point);
    //设置颜色
    void setMyColor(FontColor color);

private:
    Ui::ScorePannel *ui;
    QVector<QLabel*> m_labels;
};

#endif // SCOREPANNEL_H
