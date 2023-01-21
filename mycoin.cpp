#include "mycoin.h"

//MyCoin::MyCoin(QWidget *parent)
//    : QWidget{parent}
//{

//}

MyCoin::MyCoin(QString btnImg){
    QPixmap pix;
    bool ret = pix.load(btnImg);
    if(!ret){
        QString str = QString("load image %1 fail").arg(btnImg);
        qDebug() << str;
        return;
    }

    //设置图片固定大小
    this->setFixedSize(pix.width(), pix.height());

    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:Opx;}");

    //设置图标
    this->setIcon(pix);

    //设置图标大小
    this->setIconSize(QSize(pix.width(), pix.height()));

    //初始化定时器对象
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    //监听正面翻反面的信号并翻转金币
    connect(timer1, &QTimer::timeout, [=](){
        this->isAnimation = true;
        QPixmap pix;
        QString str = QString(":/res/Coin000%1").arg(this->min++);
        pix.load(str);

        this->setFixedSize(pix.width(), pix.height());
        this->setStyleSheet("QPushButton{border:Opx;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(), pix.height()));
        if(this->min > this->max){
            this->min = 1;
            this->isAnimation = false;
            timer1->stop();
        }
    });

    connect(timer2, &QTimer::timeout, [=](){
        this->isAnimation = true;
        QPixmap pix;
        QString str = QString(":/res/Coin000%1").arg(this->max--);
        pix.load(str);
        this->setFixedSize(pix.width(), pix.height());
        this->setStyleSheet("QPushButton{border:Opx;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(), pix.height()));
        if(this->max < this->min){
            this->max = 8;
            this->isAnimation = false;
            timer2->stop();
        }
    });
}

//保证金币翻转动画完整执行不受多次点击影响以及游戏胜利后终止后续操作
void MyCoin::mousePressEvent(QMouseEvent *e){
    if(this->isAnimation || this->isWin){
        return;
    }
    else{
        QPushButton::mousePressEvent(e);
    }
}

void MyCoin::changeFlag(){
    //如果是正面，翻成反面
    if(this->flag){
        //开始正面翻成反面的
        timer1->start(30);
        this->flag = false;
    }
    //反面翻正面
    else{
        timer2->start(30);
        this->flag = true;
    }
}
