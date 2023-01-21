#include "playscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QLabel>
#include "mycoin.h"
#include "dataconfig.h"
#include <QPropertyAnimation>
#include <QRect>
#include <QSoundEffect>
//playscene::playscene(QWidget *parent)
//    : QMainWindow{parent}
//{

//}

playscene::playscene(int stage){
    QString str1 = QString("关卡%1").arg(stage);
    QString str2 = QString("Level: %1").arg(stage);
    qDebug()<<str1;

    this->levelIndex = str2;

    this->stage = stage;

    //初始化游戏场景
    this->setFixedSize(400,565);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle(str1);

    //创建菜单栏
    QMenuBar *bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu *startMenu = bar->addMenu("开始");

    //创建退出菜单项
    QAction * quitAction = startMenu->addAction("退出");

    QSoundEffect *flipSound = new QSoundEffect;
    flipSound->setSource(QUrl::fromLocalFile(":/res/CoinFlipSound.wav"));

    QSoundEffect *winSound = new QSoundEffect;
    winSound->setSource(QUrl::fromLocalFile(":/res/LevelWinSound.wav"));

    //点击退出实现退出游戏
    connect(quitAction, &QAction::triggered, [=](){
        this->close();
    });

    //返回按钮
    MyPushButton* backbtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backbtn->setParent(this);
    backbtn->move(this->width()-backbtn->width(), this->height()-backbtn->height());

    //点击返回
    connect(backbtn, &MyPushButton::clicked, [=](){
        //告诉主场景返回,主场景监听ChooseLevelScene的返回按钮
        emit this->playSceneBack();
    });

    //显示当前关卡数
    QLabel *label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(25);
    //将字体设置到标签控件中
    label->setFont(font);
    label->setText(this->levelIndex);
    label->setGeometry(30,this->height()-60,140,50);

    dataConfig config;
    //初始化每关的二维数组
    for(int i = 0; i<4; i++){
        for(int j = 0; j<4; j++){
            this->gameArray[i][j] = config.mData[this->stage][i][j];
        }
    }

    //胜利图片显示
    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(), tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width() - tmpPix.width())*0.5,-tmpPix.height());

    //显示金币背景图案
    for(int i = 0; i<4;i++){
        for(int j=0; j<4;j++){
            //绘制背景图片
            QPixmap pix = QPixmap(":/res/BoardNode(1).png");
            QLabel * label = new QLabel;
            label->setGeometry(0,0,pix.width(),pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(95+i*50, 205 + j*50);

            //创建金币
            QString str;
            if(this->gameArray[i][j] == 1){
                str = ":/res/Coin0001.png";
            }
            else{
                str = ":/res/Coin0008.png";
            }

            MyCoin *coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(97+i*50, 207+j*50);

            //给金币属性复制
            coin->posX = i;
            coin->posY = j;
            coin->flag = this->gameArray[i][j];
            coinBtn[i][j] = coin;

            //点击金币，进行翻转
            connect(coin, &MyCoin::clicked, [=](){
                flipSound->play();
                //在翻转瞬间防止鼠标传入其他硬币，在点击过程完成时释放
                for(int i = 0; i<4; i++){
                    for(int j=0; j<4; j++){
                        this->coinBtn[i][j]->isWin = true;
                    }
                }

                coin->changeFlag();
                this->gameArray[i][j] = this->gameArray[i][j] == 0? 1 : 0;
                //翻转周围硬币
                QTimer::singleShot(300,this, [=](){
                    if(coin->posX + 1 <=3){ //右侧翻转边界
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY] = this->gameArray[coin->posX+1][coin->posY] == 0? 1 : 0;
                    }
                    if(coin->posX - 1 >=0){ //左侧翻转边界
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY] = this->gameArray[coin->posX-1][coin->posY] == 0? 1 : 0;
                    }
                    if(coin->posY + 1 <=3){ //上侧翻转边界
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY+1] = this->gameArray[coin->posX][coin->posY+1] == 0? 1 : 0;
                    }
                    if(coin->posY - 1 >=0){ //下侧翻转边界
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY-1] = this->gameArray[coin->posX][coin->posY-1] == 0? 1 : 0;
                    }

                    //按钮禁用恢复
                    for(int i = 0; i<4; i++){
                        for(int j=0; j<4; j++){
                            this->coinBtn[i][j]->isWin = false;
                        }
                    }

                    //判断是否胜利
                    this->isWin = true;
                    for(int i = 0; i < 4; i++){
                        for(int j = 0; j < 4;j ++){
                            if(coinBtn[i][j]->flag == false){ //只要有一个翻面就依然没有胜利
                                this->isWin = false;
                                break;
                            }
                        }
                    }
                    if(this->isWin == true){
                        winSound->play();
                        qDebug() << "game win!";
                        //游戏胜利后停止鼠标后续点击
                        for(int i = 0; i < 4; i++){
                            for(int j = 0; j < 4;j ++){
                                coinBtn[i][j]->isWin = true;
                            }
                        }

                        //将胜利的图片移动
                        QPropertyAnimation* animation = new QPropertyAnimation(winLabel, "geometry");
                        //设置事件间隔
                        animation->setDuration(1000);
                        //设置开始位置
                        animation->setStartValue(QRect(winLabel->x(), winLabel->y(), winLabel->width(), winLabel->height()));
                        //设置结束位置
                        animation->setEndValue(QRect(winLabel->x(), winLabel->y()+130, winLabel->width(), winLabel->height()));
                        //设置缓和曲线
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        //执行动画
                        animation->start();
                    }
                });

            });
        }
    }
}

void playscene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    //拉伸图片适合界面
    painter.drawPixmap(0,0,this->width(), this->height(), pix);

    //画背景上图标
    pix.load(":/res/Title.png");
    painter.drawPixmap(50,30,pix);
}

