#include "widget.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QIntValidator>
#include <QString>
#include <QDebug>
#include <QFont>
#include <QMessageBox>
#include <QApplication>


//extern bool distribute[1];
//extern int memory[1];
//extern int algo[1];
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //设置窗口固定大小
    setFixedSize(400,170);

    QFont font1("STKaiti",25,75);
    QFont font2("STKaiti",18,75);
    QLabel * welcome = new QLabel;
    welcome->setText("动态分区分配模拟器");
    welcome->setFont(font1);
    welcome->move(80,0);
    welcome->setParent(this);

    //设置文本提示
    QLabel * hint = new QLabel;
    hint->setText("自定义内存大小");
    hint->setFont(font2);
    hint->move(25,48);
    hint->setParent(this);

    //设置输入文本框 获取自定义内存大小
    QLineEdit * MemorySize = new QLineEdit;
    MemorySize->setPlaceholderText("范围0-9999");
    MemorySize->setGeometry(180,48,90,20);
    MemorySize->setParent(this);
    MemorySize->setValidator(new QIntValidator(0,9999,this));


    //设置确认按钮
    QPushButton *confirmBtn = new QPushButton;
    confirmBtn->setParent(this);
    confirmBtn->setText("确认");
    confirmBtn->move(300,44);



    //设置连接存储用户输入的内存大小
    connect(confirmBtn,&QPushButton::clicked,this,[=](){
        QString memory1 = MemorySize->text();
        if(memory1 == "")
        {
            QMessageBox::critical(NULL,"提示","请输入合法数字",QMessageBox::Yes);
            distribute[0] = false;
            qDebug() << "未输入合法数字！";
        }
        else
        {
            QMessageBox::information(NULL, "内存分配", "自定义内存大小分配成功！", QMessageBox::Yes, QMessageBox::Yes);
            memory = memory1.toInt();
            distribute[0] = true;
        }
    });

    //选择分配方式
    QLabel * choose = new QLabel;
    choose->setText("请选择分配算法");
    choose->move(25,90);
    choose->setFont(font2);
    choose->setParent(this);

    QPushButton * firstAdaptBtn = new QPushButton;
    firstAdaptBtn->setText("首次适应算法");
    firstAdaptBtn->move(20,120);
    firstAdaptBtn->setParent(this);
    //跳转

    connect(firstAdaptBtn,&QPushButton::clicked,[=](){
        if(distribute[0] == false)
        {
            QMessageBox::critical(NULL,"错误","没有指定内存大小！",QMessageBox::Yes);
        }
        else
        {
            algorithmScene = new AdaptAlgo;
            connect(this,SIGNAL(chooseAlgo(int)),algorithmScene,SLOT(getAlgo(int)));
            emit chooseAlgo(1);
            connect(this,SIGNAL(pushData(int)),algorithmScene,SLOT(getData(int)));
            emit pushData(memory);
            this->hide();
            algorithmScene->show();

            //监听返回按钮的信号
            connect(algorithmScene,&AdaptAlgo::backMainScene,this,[=](){
                algorithmScene->hide();
                this->show();
            });
        }
    });

    QPushButton * bestAdaptBtn = new QPushButton;
    bestAdaptBtn->setText("最佳适应算法");
    bestAdaptBtn->move(160,120);
    bestAdaptBtn->setParent(this);
    connect(bestAdaptBtn,&QPushButton::clicked,[=](){
        if(distribute[0] == false)
        {
            QMessageBox::critical(NULL,"错误","没有指定内存大小！",QMessageBox::Yes);
        }
        else
        {
            algorithmScene = new AdaptAlgo;
            connect(this,SIGNAL(chooseAlgo(int)),algorithmScene,SLOT(getAlgo(int)));
            emit chooseAlgo(2);
            connect(this,SIGNAL(pushData(int)),algorithmScene,SLOT(getData(int)));
            emit pushData(memory);
            this->hide();
            algorithmScene->show();

            //监听返回按钮的信号
            connect(algorithmScene,&AdaptAlgo::backMainScene,this,[=](){
                algorithmScene->hide();
                this->show();
            });
        }
    });

    //退出按钮
    QPushButton * quitBtn = new QPushButton;
    quitBtn->setText("退出");
    quitBtn->move(300,120);
    quitBtn->setParent(this);
    connect(quitBtn,&QPushButton::clicked,[=](){
        QApplication *app;
        app->quit();
    });
}




Widget::~Widget()
{
}

