#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QSoundEffect>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //配置主场景

    //设置固定大小
    setFixedSize(400,565);

    //设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));

    //设置标题
    setWindowTitle("翻金币主场景");

    //退出按钮实现
    connect(ui->actionquit, &QAction::triggered, [=](){
        this->close();
    });

    //准备开始按钮的音效
    QSoundEffect *startSound = new QSoundEffect;
    startSound->setSource(QUrl::fromLocalFile(":/res/TapButtonSound.wav"));
//    startSound->setLoopCount(1);  //循环次数
//    startSound->setVolume(0.25f);

    //开始按钮
    MyPushButton *startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5 - startBtn->width()*0.5, this->height()*0.7);

    chooseScene = new ChooseLevelScene;

    connect(startBtn, &MyPushButton::clicked, [=](){
        //qDebug() << "click start";
        startSound->play();
        startBtn->zoom1();
        startBtn->zoom2();

        //延时进入选择关卡场景
        QTimer::singleShot(300, this, [=](){
            //设置选择关卡场景位置
            chooseScene->setGeometry(this->geometry());

            this->hide();
            //显示选择关卡场景
            chooseScene->show();
        });
    });

    //监听选择关卡的返回按钮的信号
    connect(chooseScene, &ChooseLevelScene::chooseSceneBack, this, [=](){
        //延时返回
        this->setGeometry(chooseScene->geometry());
        QTimer::singleShot(300, this, [=](){
            chooseScene->hide();
            //显示选择关卡场景
            this->show();
        });
    });
}

void MainScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    //拉伸图片适合界面
    painter.drawPixmap(0,0,this->width(), this->height(), pix);

    //画背景上图标
    pix.load(":/res/Title.png");
    painter.drawPixmap(50,30,pix);

}

MainScene::~MainScene()
{
    delete ui;
}

