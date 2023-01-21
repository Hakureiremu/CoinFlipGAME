#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include "playscene.h"
#include <QSoundEffect>

ChooseLevelScene::ChooseLevelScene(QWidget *parent)
    : QMainWindow{parent}
{
    //配置选择关卡场景
    this->setFixedSize(400,565);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("选择关卡场景");

    //创建菜单栏
    QMenuBar *bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu *startMenu = bar->addMenu("开始");

    //创建退出菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击退出实现退出游戏
    connect(quitAction, &QAction::triggered, [=](){
        this->close();
    });

    //返回按钮
    MyPushButton* backbtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backbtn->setParent(this);
    backbtn->move(this->width()-backbtn->width(), this->height()-backbtn->height());

    QSoundEffect *backSound = new QSoundEffect;
    backSound->setSource(QUrl::fromLocalFile(":/res/BackButtonSound.wav"));

    //点击返回
    connect(backbtn, &MyPushButton::clicked, [=](){
        //播放返回按钮音效
        backSound->play();

        //告诉主场景返回,主场景监听ChooseLevelScene的返回按钮
        emit this->chooseSceneBack();
    });



    //创建选择关卡按钮
    for(int i = 0; i<20; i++){
        //关卡按钮
        MyPushButton *menubtn = new MyPushButton(":/res/LevelIcon.png");
        menubtn->setParent(this);
        menubtn->move(60+i%4 * 70, 150+i/4 * 70);

        //显示关卡数字
        QLabel *label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menubtn->width(), menubtn->height());
        label->setText(QString::number(i+1));
        label->move(59+i%4 * 70, 149+i/4 * 70);

        //设置label的文字对齐方式 水平居中和垂直居中
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        //设置让鼠标进行穿透
        label->setAttribute(Qt::WA_TransparentForMouseEvents);

        connect(menubtn, &QPushButton::clicked, [=](){
            QString str = QString("choose %1").arg(i+1);
            qDebug() << str;

            //进入游戏场景
            this->hide();
            play = new playscene(i+1);
            play->setGeometry(this->geometry());
            play->show();

            //监听游戏界面的返回信号
            connect(play, &playscene::playSceneBack, this, [=](){
                //延时返回
                QTimer::singleShot(300, this, [=](){
                    this->setGeometry(play->geometry());
                    delete play;
                    play = NULL;
                    //显示选择关卡场景
                    this->show();
                });
            });
        });
    }

}

void ChooseLevelScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    //拉伸图片适合界面
    painter.drawPixmap(0,0,this->width(), this->height(), pix);

    //画背景上图标
    pix.load(":/res/Title.png");
    painter.drawPixmap(50,30,pix);
}
