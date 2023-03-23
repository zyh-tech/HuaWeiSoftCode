#pragma once
#include "data.h"

double MAP_UNIT_LEN = 0.5;      //地图单位格子的宽度0.5米
int MAP_LEN = 50;              //地图尺寸50米
int xCharNum = 100;             //字符地图的行数
int yCharNum = 100;              //字符地图的列数


//读取判题器发送的OK
bool readUntilOK() {
    char line[10];
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            return true;
        }
        //do something
    }
    return false;
}

//程序初始化，读取地图
void readMap() {

    WorkFactory* workfactory = new WorkFactory;    //声明用于生产特定工作台类型的工厂.

    char intmp;
    for (int i = 0; i < xCharNum; i++) {
        posy = 50 - 0.5 * MAP_UNIT_LEN - i * MAP_UNIT_LEN;
        for (int j = 0; j < yCharNum; j++) {
            posx = 0.5 * MAP_UNIT_LEN + j * MAP_UNIT_LEN;

            intmp = getchar();
            if (intmp == '.') continue;      //空地跳过
            if (intmp == 'A') {
                //读到了机器
                RobotId++;
                Robot* robot = new Robot(posx, posy);
                RobotList.push_back(robot);
            }
            else {
                //读到了工作台
                WorkId++;
                WorkType = intmp - '0';
                WorkBase* workbase = workfactory->create(WorkId, WorkType, posx, posy, RemainTime, MaterialState, ProductIsOk);
                WorkList.push_back(workbase);
                for(int i=1; i<8 ;i++)
                    MasterialCells[i] += (workbase->StateMask>>i & 1);
            }
        }
        intmp = getchar();          //读取每一行的换行符
    }
}