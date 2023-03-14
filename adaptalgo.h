#ifndef ADAPTALGO_H
#define ADAPTALGO_H

#include <QWidget>
#include <QPainter>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>



class AdaptAlgo : public QWidget
{
    Q_OBJECT
public:

    QLineEdit * memsize = new QLineEdit;

    explicit AdaptAlgo(QWidget *parent = nullptr);
    int memory = 0;
    int chooseAlgo = -1;

    bool initSuccess = false;//是否初始化
    bool huizhi = false;//是否需要更新图像显示

    struct partitionNode
    {
        int address;    //起始地址
        int size;       //分区大小
        int taskNum;    //作业号
        QString state;  //分区状态
        partitionNode* pre;  //分区前指针
        partitionNode* next;    //分区后指针
    }partitionHead;


    void paintEvent(QPaintEvent *event);
    int bestAdapt(int taskNum,int size);   //最佳适应算法
    int firstAdapt(int taskNum,int size);  //首次适应算法
    void initPartition();   //初始化空闲分区
    int recycleMem(int taskNum);  //回收内存
    void showMem();     //展示内存状态

signals:
    //自定义信号 点击返回信号
    void backMainScene();

public slots:
    void getData(int get);
    void getAlgo(int algo);
};

#endif // ADAPTALGO_H
