#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H
#include "playscene.h"
#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyPushButton(QWidget *parent = nullptr);
    //构造函数 参1 正常显示图片路径 参2 按下后显示图片的路径
    MyPushButton(QString normalImg, QString pressImg = "");
    //成员属性 保存用户传入的默认显示路径以及按下后显示的图片
    QString normalImgPath;
    QString pressImgPath;

    //弹跳特效
    void zoom1();
    void zoom2();

    //重写按钮按下和释放事件
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:

};

#endif // MYPUSHBUTTON_H
