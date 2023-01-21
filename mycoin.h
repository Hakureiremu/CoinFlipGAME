#ifndef MYCOIN_H
#define MYCOIN_H
#include <QTimer>
#include <QPushButton>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
//    explicit MyCoin(QWidget *parent = nullptr);
    MyCoin(QString btnImg);

    //金币属性
    int posX;
    int posY;
    bool flag; //正反标志

    //改变标志的方法
    void changeFlag();

    QTimer *timer1;
    QTimer *timer2;
    int min = 1;
    int max = 8;

    bool isAnimation = false; //标识翻转动画是否正在进行
    //重写鼠标按下事件
    void mousePressEvent(QMouseEvent *e);
    bool isWin = false;

signals:

};

#endif // MYCOIN_H
