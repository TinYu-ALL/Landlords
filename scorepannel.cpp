#include "scorepannel.h"
#include "ui_scorepannel.h"


const QString MYCOLOR[]={"black","white","blue","red","green"};
ScorePannel::ScorePannel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScorePannel)
{
    ui->setupUi(this);
    m_labels << ui->m_user << ui->robotLeft << ui->robotRight
             << ui->mScore << ui->leftScore << ui->rightScore
             << ui->score1 <<ui->score2 << ui->score3;

}

ScorePannel::~ScorePannel()
{
    delete ui;
}

void ScorePannel::setScore(int left, int right, int user)
{
    ui->leftScore->setText(QString::number(left));
    ui->rightScore->setText(QString::number(right));
    ui->mScore->setText(QString::number(user));
}

void ScorePannel::setMyFont(int point)
{
    QFont font("Segoe Print",point,QFont::Bold);
    for(int i=0;i<m_labels.size();++i)
    {
        m_labels.at(i)->setFont(font);
    }
}

void ScorePannel::setMyColor(FontColor color)
{
    QString style = QString("QLabel{color:%1}").arg(MYCOLOR[color]);
    for(int i=0;i<m_labels.size();++i)
    {
        m_labels.at(i)->setStyleSheet(style);
    }
}
