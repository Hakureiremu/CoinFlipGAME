#ifndef PLAYSCENE_H
#define PLAYSCENE_H
#include "mycoin.h"
#include <QString>
#include <QMainWindow>

class playscene : public QMainWindow
{
    Q_OBJECT
public:
//    explicit playscene(QWidget *parent = nullptr);
    //重载构造函数
    int stage;
    QString levelIndex;
    playscene(int stage);


    //重写画图事件
    void paintEvent(QPaintEvent *);

    int gameArray[4][4]; //二维数组 维护每个关卡的具体数据

    MyCoin *coinBtn[4][4];

    //判断是否胜利
    bool isWin;

signals:
    void playSceneBack();
};

#endif // PLAYSCENE_H
