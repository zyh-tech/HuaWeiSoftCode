#pragma once
#include <iostream>
#include "Robot.h"
#include "Work.h"
using namespace std;

//一些全局变量及其初始化
int frameID = 0;                //当前帧数
int cash = 200000;              //当前金钱
int K = 1;                          //工作台数量
double posx = -1;               //每个字符位置
double posy = -1;

int WorkId = -1;                //工作台序号
int WorkType = -1;              //工作台类型
int RemainTime = -1;                  //工作台剩余生产时间
int MaterialState = 0;          //工作台原材料状态
bool ProductIsOk = 0;           //工作台产品格状态
int ServerRobot = -1;           //表明该工作台当前正被哪一个id的机器人服务
int workid = -1;         //-1：表示当前没有处于任何工作台附近
                        //[0,工作台总数-1] ：表示某工作台的下从 0 开始，按输入顺序定。


int RobotNum = 4;                //机器人数量
int CarryType = 0;              //携带物品的类型[0-7]，0表示未携带物品
int RobotWorkId = -1;           //机器人所处的工作台id
int RobotId = -1;               //机器人序号
double TimeValue = 1;          //机器人时间价值系数
double CollideValue = 1;        //机器人碰撞价值系数
double PalStance = 0;           //机器人角速度
double LinearVel[2] = { 0,0 };     //机器人线速度
double Direction = 0;            //机器人朝向
int TargetWorkId = -1;           //机器人目标工作台id


//场上格子状态，总数、已经被放置占用、已经被预定占用，MasterialCells[i]表示材料类型i，MasterialCells[0]没有用
int MasterialCells[8] = {0,0,0,0,0,0,0,0}; 
int MasterialCellsUsed[8] = {0,0,0,0,0,0,0,0};
int MasterialCellsBooked[8] = {0,0,0,0,0,0,0,0};

int robotchangflag[4] = {1,1,1,1};

//存放工作台和各机器人的容器
vector<Robot*> RobotList;
vector<WorkBase*> WorkList;
vector<double> curCommond(2, 0);

int FPS = 50;                 
double TimeOfFrame = 0.02;              //一帧的时间是0，02秒