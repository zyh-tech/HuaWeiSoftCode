#pragma once
#include <vector>
#include "data.h"
using namespace std;

class WorkBase
{
public:
    WorkBase() {};
    ~WorkBase() {};
    WorkBase(int id, int type, double posx, double posy) {
        this->id = id;
        this->type = type;
        this->posx = posx;
        this->posy = posy;
    }

    void update(int type, double posx, double posy, int RemainTime, int MaterialState, bool ProductIsOk) {
        this->type = type;
        this->posx = posx;
        this->posy = posy;
        this->RemainTime = RemainTime;
        this->MaterialState = MaterialState;
        this->ProductIsOk = ProductIsOk;
    }

    //获取当前工作台到（robotPosx，robotPosy）的距离的平方（因为只需要比较距离大小，所以可以省掉开方节约时间）
    double getDistance(const double& robotPosx, const double& robotPosy) {
        return (robotPosx - posx) * (robotPosx - posx) + (robotPosy - posy) * (robotPosy - posy);
    }

        //该函数用于判断机器人能否去一个即将生产完的机器
    bool ProductToBeOK(const double& posx,const double& posy){
        double dis = sqrt(this->getDistance(posx,posy));
        //计算机器人走这个距离需要多少帧, 25是机器人加速度的一半
        int PredictFrame =  ceil(dis / 6 / 0.02) + 15;
        if( this->RemainTime > 0 && PredictFrame > this->RemainTime){
            return true;
        }else{
            return false;
        }
    }


    int id = -1;                       //工作台id，依照输入顺序来定
    int type = 0;                     //工作台类型[1,9]
    int RemainTime = -1;                     //剩余生产时间，单位是帧数 -1表示没有生产。0：表示生产因输出格满而阻塞。>=0：表示剩余生产帧数。
    int MaterialState = 0;            //材料格状态 二进制位表描述，例如 48(110000)表示拥有物品 4 和 5。
    bool ProductIsOk = 0;              //0：表示无。1：表示有

    double posx = -1;                  //工作台坐标
    double posy = -1;
    int ProductServerRobot = -1;              //表明该工作台当前正被哪一个id的机器人服务
    int StateMask = -1;
    int MaterialServerRobot[8] = {-1,-1,-1,-1,-1,-1,-1,-1};  //MaterialServerRobot[i]表示当前工作台存放号材料的位置被哪个机器人预定
};


class WorkType1 : public WorkBase {
public:
    WorkType1(int id, int type, double posx, double posy,int StateMask) {
        this->id = id;
        this->type = type;
        this->posx = posx;
        this->posy = posy;
        this->StateMask = StateMask;
    }

    int BuyPrice = 3000;
    int SellPrice = 6000;
    int workT = 50;               //工作周期，单位是frame
    
};


class WorkType2 :public  WorkBase {
public:
    WorkType2(int id, int type, double posx, double posy,int StateMask) {
        this->id = id;
        this->type = type;
        this->posx = posx;
        this->posy = posy;
        this->StateMask = StateMask;
    }

    int BuyPrice = 4400;
    int SellPrice = 7600;
    int workT = 50;               //工作周期，单位是frame
};



class WorkType3 : public WorkBase {
public:
    WorkType3(int id, int type, double posx, double posy,int StateMask) {
        this->id = id;
        this->type = type;
        this->posx = posx;
        this->posy = posy;
        this->StateMask = StateMask;
    }

    int BuyPrice = 5800;
    int SellPrice = 9200;
    int workT = 50;               //工作周期，单位是frame
};



class WorkType4 : public WorkBase {
public:
    WorkType4(int id, int type, double posx, double posy,int StateMask) {
        this->id = id;
        this->type = type;
        this->posx = posx;
        this->posy = posy;
        this->StateMask = StateMask;
    }

    int BuyPrice = 15400;
    int SellPrice = 22500;
    int workT = 500;               //工作周期，单位是frame
};



class WorkType5 : public WorkBase {
public:
    WorkType5(int id, int type, double posx, double posy,int StateMask) {
        this->id = id;
        this->type = type;
        this->posx = posx;
        this->posy = posy;
        this->StateMask = StateMask;
    }

    int BuyPrice = 17200;
    int SellPrice = 25000;
    int workT = 500;               //工作周期，单位是frame
};



class WorkType6 : public WorkBase {
public:
    WorkType6(int id, int type, double posx, double posy,int StateMask) {
        this->id = id;
        this->type = type;
        this->posx = posx;
        this->posy = posy;
        this->StateMask = StateMask;
    }

    int BuyPrice = 19200;
    int SellPrice = 27500;
    int workT = 500;               //工作周期，单位是frame
};



class WorkType7 : public WorkBase {
public:
    WorkType7(int id, int type, double posx, double posy,int StateMask) {
        this->id = id;
        this->type = type;
        this->posx = posx;
        this->posy = posy;
        this->StateMask = StateMask;
    }

    int BuyPrice = 76000;
    int SellPrice = 105000;
    int workT = 1000;               //工作周期，单位是frame
};



class WorkType8 : public WorkBase {
public:
    WorkType8(int id, int type, double posx, double posy,int StateMask) {
        this->id = id;
        this->type = type;
        this->posx = posx;
        this->posy = posy;
        this->StateMask = StateMask;
    }

    int workT = 1;               //工作周期，单位是frame
    int SellPrice = 105000;
};



class WorkType9 : public WorkBase {
public:
    WorkType9(int id, int type, double posx, double posy, int StateMask) {
        this->id = id;
        this->type = type;
        this->posx = posx;
        this->posy = posy;
        this->StateMask = StateMask;
    }

    int workT = 1;               //工作周期，单位是frame
    int SellPrice[9] = { 0,6000,7600,9200,22500,25000,27500,10500 };
};


//所有工作台共有属性
class WorkFactory {
    //使用work工厂按照传入的参数生成特定的工作台类型
public:
    WorkBase* create(int id, int type, double posx, double posy, int RemainTime, int MaterialState, bool ProductIsOk) {
        switch (type)
        {
        case 1: return new WorkType1(id, type, posx, posy, 1); break;
        case 2: return new WorkType2(id, type, posx, posy, 1); break;
        case 3: return new WorkType3(id, type, posx, posy, 1); break;
        case 4: return new WorkType4(id, type, posx, posy, 6); break;
        case 5: return new WorkType5(id, type, posx, posy, 10); break;
        case 6: return new WorkType6(id, type, posx, posy, 12); break;
        case 7: return new WorkType7(id, type, posx, posy, 112); break;
        case 8: return new WorkType8(id, type, posx, posy, 128); break;
        case 9: return new WorkType9(id, type, posx, posy, 255); break;
        default: cout << "工作台类型错误"; return nullptr;
        }
    }
};
