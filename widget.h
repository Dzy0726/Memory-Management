#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <adaptalgo.h>
#include <QLineEdit>



class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);  //构造函数
    ~Widget();  //析构函数

     AdaptAlgo * algorithmScene = NULL; //算法实现界面

     int algoChoose = -1;   //算法选择变量
                            //-1未选择  1首先适配算法  2最佳适配算法
     int memory = 0;        //自定的内存大小
     bool distribute[1] = {false};  //是否进行内存分配

signals:
     void pushData(int data);   //信号：自定义内存大小
     void chooseAlgo(int algoChoose);   //信号：算法选择

};
#endif // WIDGET_H
