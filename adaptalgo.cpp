#include "adaptalgo.h"
#include <QDebug>
#include <widget.h>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QApplication>
#include <QPainter>
#include <QMessageBox>
#include <QValidator>


AdaptAlgo::AdaptAlgo(QWidget *parent) : QWidget(parent)
{

    this->setFixedSize(600,400);
    this->setWindowTitle("分配内存模拟");
    //地址标签
    QLabel *addr = new QLabel;
    addr->setText("地址");
    addr->setParent(this);
    addr->move(10,170);

    QLabel *taskID = new QLabel;
    taskID->setText("块号");
    taskID->setParent(this);
    taskID->move(50,170);

    //提示标签
    QLabel *hint = new QLabel;
    hint->setText("绿色表示未使用，红色表示已使用");
    hint->setParent(this);
    hint->move(170,170);

    //创建返回按钮
    QPushButton* backBtn = new QPushButton;
    backBtn->setText("返回");
    backBtn->setParent(this);
    backBtn->move(this->width() -140,this->height() - 40);
    connect(backBtn,&QPushButton::clicked,[=](){
    emit this->backMainScene();
    });

    //创建退出按钮
    QPushButton* quitBtn = new QPushButton;
    quitBtn->setText("退出");
    quitBtn->setParent(this);
    quitBtn->move(this->width() -80,this->height() - 40);
    connect(quitBtn,&QPushButton::clicked,[=](){
        QApplication *app;
        app->quit();
     });

    //分配内存控件
    QFont font2("STKaiti",20,75);
    QFont font3("STKaiti",18,75);
    QLabel *distribLabel = new QLabel;
    distribLabel->setText("分配内存");
    distribLabel->setFont(font2);
    distribLabel->setParent(this);
    distribLabel->move(20,80);

    QLabel * num1 = new QLabel;
    num1->setText("内存块号");
    num1->setParent(this);
    num1->setFont(font3);
    num1->move(150,80);

    QLineEdit * memnum1 = new QLineEdit;
    memnum1->setGeometry(230,80,60,20);
    memnum1->setParent(this);
    memnum1->setPlaceholderText("0~999");
    memnum1->setValidator(new QIntValidator(0,999,this));



    QLabel * size = new QLabel;
    size->setText("内存块大小");
    size->setParent(this);
    size->setFont(font3);
    size->move(300,80);



    QPushButton * distribBtn = new QPushButton;
    distribBtn->setText("分配");
    distribBtn->setParent(this);
    distribBtn->move(480,75);

    //分配监听器
    connect(distribBtn,&QPushButton::clicked,this,[=](){
        bool successDis = false;
        QString kuaiHao = memnum1->text();
        QString kuaiMem = memsize->text();
        qDebug() << "块号："<< kuaiHao.toInt() << "块内存大小" << kuaiMem.toInt();
        if(kuaiHao == "")
        {
            QMessageBox::critical(NULL,"提示","请输入块号",QMessageBox::Yes);
            successDis = false;
        }
        else if(kuaiMem.toInt() > memory)
        {
            QMessageBox::critical(NULL,"提示","分配内存不能大于总内存！",QMessageBox::Yes);
            successDis = false;
        }
        else if(kuaiMem.toInt() == 0)
        {
            QMessageBox::critical(NULL,"提示","分配内存不能为空或为0！",QMessageBox::Yes);
            successDis = false;
        }
        else
        {
            successDis = true;
        }
        if(initSuccess == false && successDis == true)
        {
            initPartition();
            initSuccess = true;
        }
        if(chooseAlgo == 1 && successDis == true)
        {

            qDebug() << "首先适应算法：开始分配" ;
            firstAdapt(kuaiHao.toInt(),kuaiMem.toInt());
            showMem();
        }
        else if(chooseAlgo == 2 && successDis == true)
        {
            qDebug() << "最佳适应算法：开始分配" ;
            bestAdapt(kuaiHao.toInt(),kuaiMem.toInt());
            showMem();
        }
    });

    //回收内存控件
    QLabel *recycleLabel = new QLabel;
    recycleLabel->setText("回收内存");
    recycleLabel->setFont(font2);
    recycleLabel->setParent(this);
    recycleLabel->move(20,120);

    QLabel * num2 = new QLabel;
    num2->setText("内存块号");
    num2->setParent(this);
    num2->setFont(font3);
    num2->move(150,120);

    QLineEdit * memnum2 = new QLineEdit;
    memnum2->setGeometry(230,120,60,20);
    memnum2->setParent(this);
    memnum2->setPlaceholderText("0~999");
    memnum2->setValidator(new QIntValidator(0,999,this));


    QPushButton * recycleBtn = new QPushButton;
    recycleBtn->setText("回收");
    recycleBtn->setParent(this);
    recycleBtn->move(480,115);

    //回收监听器
    connect(recycleBtn,&QPushButton::clicked,this,[=](){
        QString huiShou = memnum2->text();
        if(huiShou == "")
        {
            QMessageBox::critical(NULL,"提示","请输入内存块号",QMessageBox::Yes);
        }
        else
        {
            qDebug() << "回收内存块号为：" << huiShou.toInt();
            recycleMem(huiShou.toInt());
            showMem();
        }
    });


    //初始化
    QPushButton * initBtn = new QPushButton;
    initBtn->setText("初始化");
    initBtn->setParent(this);
    initBtn->move(480,190);

    connect(initBtn,&QPushButton::clicked,this,[=](){
        QMessageBox::information(NULL,"初始化","初始化成功！",QMessageBox::Yes);
        initPartition();
    });
};

void AdaptAlgo::getData(int get)
{
    QFont font2("STKaiti",25,75);
    memory = get;
    qDebug() <<"接收信号！" << memory;
    QLabel * memLabel = new QLabel;
    memLabel -> setText("内存大小："+QString::number(memory));
    memLabel->setParent(this);
    memLabel->setFont(font2);
    memLabel->move(350,20);

    memsize->setGeometry(400,80,60,20);
    memsize->setParent(this);
    memsize->setValidator(new QIntValidator(0,memory,this));

}

void AdaptAlgo::getAlgo(int algo)
{
    chooseAlgo = algo;
    QLabel * welcome = new QLabel;
    QFont font1("STKaiti",25,75);
    welcome->setFont(font1);
    welcome->setParent(this);
    welcome->move(20,20);
        if(algo == 1)
        {
            welcome->setText("当前选择：首次适应算法");
        }
        else if(algo == 2)
        {
            welcome->setText("当前选择：最佳适应算法");
        }
        else
        {
            qDebug() << "算法选择未传递！";
        }
}

void AdaptAlgo::paintEvent(QPaintEvent *)
{
    QPainter painterxmx(this);
    QPen pen1;
    pen1.setWidth(5);
    pen1.setColor(QColor(176,196,222));
    painterxmx.setPen(pen1);
    painterxmx.drawLine(0,60,600,60);
    painterxmx.drawLine(0,160,600,160);
    painterxmx.fillRect(90,190,360,200,QColor(0,255,0));

    QPen pen2;
    pen2.setWidth(4);
    pen2.setColor(QColor(0,0,0));
    painterxmx.setPen(pen2);


    if(huizhi == true)
    {
        int widthRec = 0;
        int begin = 0;
        int end = 0;
        partitionNode* p = partitionHead.next;
        while(p != NULL)
        {
            if(p->state == "busy")
            {
               begin = 190 + p->address*200 / memory;
               widthRec = p->size*200 / memory;
               end = begin+widthRec;
               painterxmx.drawRect(90,begin,360,widthRec);
               painterxmx.fillRect(90,begin,360,widthRec,QColor(255,0,0));

               QLabel * addrLab = new QLabel;
               QLabel * taskLabel = new QLabel;

               addrLab->setNum(p->address);
               addrLab->move(10,begin);
               addrLab->setParent(this);
               addrLab->show();

               taskLabel->setNum(p->taskNum);
               taskLabel->move(50,begin);
               taskLabel->setParent(this);
               taskLabel->show();
            }
            p = p->next;
        }
        huizhi = false;
    }


}

void AdaptAlgo::initPartition()   //初始化空闲分区
{
    partitionNode* firstNode = new partitionNode;
    firstNode->address = 0;
    firstNode->size = memory;
    firstNode->state = "free";
    firstNode->pre = &partitionHead;
    firstNode->next = NULL;

    //初始化分区头部信息
    partitionHead.pre = NULL;
    partitionHead.next = firstNode;

    QPixmap  pix;
    pix.load(":/back.jpg");
//            pix.scaled(0.8,0.8);
    QLabel * fugai = new QLabel;
    fugai->move(10,190);
    fugai->resize(70,200);
    fugai->setParent(this);
    fugai->setPixmap(pix);
    fugai->setScaledContents(true);
    fugai->raise();
    fugai->show();
}

int AdaptAlgo::firstAdapt(int taskNum,int size)  //首次适应算法
{
    qDebug() << "进入首次适应算法成功！taskNum = " << taskNum << "size = " << size;
    partitionNode* p = partitionHead.next;
    while(p != NULL)
    {
        if(p->taskNum == taskNum)
        {
            QMessageBox::critical(NULL,"提示","内存块号重复请重新输入！",QMessageBox::Yes);
            return 0;
        }
        if(p->state == "free" && p->size >= size)   //从头找到第一个满足分配条件的分区
        {
            if(p->size - size <= 10) //大小正好 整块分配
            {
                p->state = "busy";
                p->taskNum = taskNum;
            }
            else //一般情况下 新建一个s链接在p后面
            {
                partitionNode* s = new partitionNode;
                s->address = p->address + size;
                s->size = p->size - size;
                s->state = "free";
                s->taskNum = -1;

                s->pre = p;
                s->next = p->next;

                if(p->next != NULL)
                {
                    p->next->pre = s;
                }
                p->next = s;
                p->state = "busy";
                p->taskNum = taskNum;
                p->size = size;
            }
            QMessageBox::information(NULL, "内存分配", "内存分配成功！", QMessageBox::Yes, QMessageBox::Yes);
            return  1;
        }
        p = p->next;
    }
    QMessageBox::critical(NULL,"提示","没有合适的内存分区，分配失败！",QMessageBox::Yes);
    return 0;
}

int AdaptAlgo::bestAdapt(int taskNum,int size)  //最佳适应算法
{
    qDebug() << "进入最佳适应算法成功！taskNum = " << taskNum << "size = " << size;
    partitionNode * bestByNow = NULL;   //截至目前为止最好的选择（区间）
    int bestSizeByNow = memory+1;       //截至目前为止最好的区间的大小
    partitionNode *p = partitionHead.next;
    while(p != NULL)        //找到最佳区间
    {
        if(p->taskNum == taskNum)
        {
            QMessageBox::critical(NULL,"提示","内存块号重复请重新输入！",QMessageBox::Yes);
            return 0;
        }
        if(p->state == "free" && p->size >=size && p->size < bestSizeByNow)
        {
            bestByNow = p;
            bestSizeByNow = p->size;
        }
        p = p -> next;
    }
    if(bestByNow != NULL)
    {
        if(bestByNow->size == size)
        {
            bestByNow->state = "busy";
            bestByNow->taskNum =taskNum;
        }
        else
        {
            partitionNode* s = new partitionNode;
            s->address = bestByNow->address + size;
            s->size = bestByNow->size - size;
            s->state = "free";
            s->taskNum = -1;

            s->pre = bestByNow;
            s->next = bestByNow->next;

            if(bestByNow->next != NULL)
            {
                bestByNow->next->pre = s;
            }
            bestByNow->next = s;
            bestByNow->state = "busy";
            bestByNow->taskNum = taskNum;
            bestByNow->size = size;
        }
        QMessageBox::information(NULL, "内存分配", "内存分配成功！", QMessageBox::Yes, QMessageBox::Yes);
        return  1;
    }
    else
    {
        QMessageBox::critical(NULL,"提示","没有合适的内存分区，分配失败！",QMessageBox::Yes);
        return 0;
    }
}

int AdaptAlgo::recycleMem(int taskNum)  //回收内存
{


    bool isRecycle = false;
    partitionNode *p = partitionHead.next;
    partitionNode *s;
    while(p != NULL)
    {
        if(p->state == "busy" && p->taskNum == taskNum)
        {
            QLabel * fugai = new QLabel;
            QPixmap  pix;
            pix.load(":/back.jpg");
//            pix.scaled(0.8,0.8);
            fugai->move(10,190 + p->address*200 / memory);
            fugai->resize(70, p->size*200 / memory+190 + p->address*200 / memory);
            fugai->setParent(this);
            fugai->setPixmap(pix);
            fugai->setScaledContents(true);
            fugai->raise();

            isRecycle = true;
            //第一种可能：要合并上下两个分区 先合并上面的 再合并下面的
            if((p->pre != &partitionHead && p->pre->state == "free")
                && (p->next != NULL && p->next->state == "free"))
            {
                qDebug() << "当前信息块号" << p->taskNum << "状态" << p->state;
                s = p;
                p = p->pre;
                p->size = s->size+p->size;
                p->next = s->next;
                s->next->pre = p;
                delete s;

                s = p->next;
                p->size = p->size+s->size;
                p->next = s->next;
                if(s->next != NULL)
                {
                    s->next->pre = p;
                }
                delete s;
            }
            //第二种可能：只合并上面的分区
            else if((p->pre != &partitionHead && p->pre->state =="free")
                    && (p->next ==  NULL || p->next->state =="busy"))
            {
                qDebug() << "当前信息块号" << p->taskNum << "状态" << p->state;
                s = p;
                p= p->pre;
                p->size = s->size+p->size;
                p->next = s->next;
                if(s->next != NULL)
                {
                    s->next->pre = p;
                }
                delete s;
            }
            //第三种可能：只合并下面的分区
            else if((p->pre == &partitionHead || p->pre->state == "busy")
                    &&(p->next != NULL && p->next->state == "free"))
            {
                qDebug() << "当前信息块号" << p->taskNum << "状态" << p->state;

                s = p->next;
                p->size = p->size+s->size;
                p->state = "free";
                p->taskNum = -1;
                p->next = s->next;
                if(s->next != NULL)
                {
                    s->next->pre = p;
                }
                delete s;
            }
            //第四种可能：上下分区都不需要合并
            else
            {
                qDebug() << "当前信息块号" << p->taskNum << "状态" << p->state;

                p->state ="free";
                p->taskNum = -1;
            }

            fugai->show();
        }
        p = p->next;
    }

    if(isRecycle == true)
    {
        QMessageBox::information(NULL, "内存回收", "内存回收成功！", QMessageBox::Yes, QMessageBox::Yes);
        return 1;
    }
    else
    {
        QMessageBox::critical(NULL,"提示","找不到目标任务，回收失败！",QMessageBox::Yes);
    }
    return 0;
}

void AdaptAlgo::showMem()     //展示内存状态
{
    partitionNode * p = partitionHead.next;
    while(p != NULL)
    {
        qDebug() << p->taskNum<< p->address << p->size << p->state ;
        p = p->next;
    }
    huizhi = true;
}
