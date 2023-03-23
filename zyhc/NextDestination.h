#pragma once

#include "data.h"
#include <queue>



int NextSellDes(const int& CarryType,const double& posx,const double& posy){

    //找距离小且能卖的
    auto cmp_distance = [&](WorkBase* a, WorkBase* b)->bool {
        return a->getDistance(posx,posy) > b->getDistance(posx,posy);
    };
    priority_queue<WorkBase*, vector<WorkBase*>, decltype(cmp_distance)> q(cmp_distance);


    // //找能卖的且当前空缺材料少的
    // auto cmp_distance = [&](WorkBase* a, WorkBase* b)->bool {
    //     if  (__builtin_popcount(a->MaterialState) < __builtin_popcount(b->MaterialState)) return true;
    //     else if  (__builtin_popcount(a->MaterialState) > __builtin_popcount(b->MaterialState)) return false;
    //     else return a->getDistance(posx,posy) > b->getDistance(posx,posy);
    // };
    // priority_queue<WorkBase*, vector<WorkBase*>, decltype(cmp_distance)> q(cmp_distance);

    //能卖指的是，这个工作台需要这个材料 ((WorkList[i]->StateMask>>CarryType) & 1) == 1
    //以及当前工作这个材料格没被占据  ((WorkList[i]->MaterialState>>CarryType) & 1) == 0
    //同时当前材料格没有被别人预定

    switch(CarryType){
        case 1:{
            for(int i=0; i<K; i++){
                if(( WorkList[i]->type == 4 || WorkList[i]->type == 5 ) && (WorkList[i]->MaterialServerRobot[CarryType] == -1) 
                    && ((WorkList[i]->MaterialState>>CarryType) & 1) == 0) q.push(WorkList[i]);
            }
            break;
        }
        case 2:{
            for(int i=0; i<K; i++){
                if(( WorkList[i]->type == 4 || WorkList[i]->type == 6 ) && (WorkList[i]->MaterialServerRobot[CarryType] == -1) 
                    && ((WorkList[i]->MaterialState>>CarryType) & 1) == 0) q.push(WorkList[i]);
            }
            break;
        }
        case 3:{
            for(int i=0; i<K; i++){
                if(( WorkList[i]->type == 5 || WorkList[i]->type == 6 ) && (WorkList[i]->MaterialServerRobot[CarryType] == -1) 
                    && ((WorkList[i]->MaterialState>>CarryType) & 1) == 0) q.push(WorkList[i]);
            }
            break;
        }
        case 4:{
            for(int i=0; i<K; i++){
                if(( WorkList[i]->type == 7 || WorkList[i]->type == 9) && (WorkList[i]->MaterialServerRobot[CarryType] == -1) 
                    && ((WorkList[i]->MaterialState>>CarryType) & 1) == 0) q.push(WorkList[i]);
            }
            break;
        }
        case 5:{
            for(int i=0; i<K; i++){
                if(( WorkList[i]->type == 7 || WorkList[i]->type == 9) && (WorkList[i]->MaterialServerRobot[CarryType] == -1) 
                    && ((WorkList[i]->MaterialState>>CarryType) & 1) == 0) q.push(WorkList[i]);
            }
            break;
        }
        case 6:{
            for(int i=0; i<K; i++){
                if(( WorkList[i]->type == 7 || WorkList[i]->type == 9) && (WorkList[i]->MaterialServerRobot[CarryType] == -1) 
                    && ((WorkList[i]->MaterialState>>CarryType) & 1) == 0) q.push(WorkList[i]);
            }
            break;
        }
        case 7:{
            for(int i=0; i<K; i++){
                if(( WorkList[i]->type == 8 || WorkList[i]->type == 9 ) && (WorkList[i]->MaterialServerRobot[CarryType] == -1) 
                    && ((WorkList[i]->MaterialState>>CarryType) & 1) == 0) q.push(WorkList[i]);
            }
            break;
        }
    }

    if (!q.empty())  return q.top()->id;
    //else return 
    else return -1;
}



bool TimeEnoughToSell(int& CurWorkId, int TargetWorkId){
    int targetx = WorkList[TargetWorkId]->posx;
    int targety = WorkList[TargetWorkId]->posy;
    double dis = sqrt(WorkList[CurWorkId]->getDistance(targetx,targety));
    //计算机器人走这个距离需要多少帧, 25是机器人加速度的一半
    int PredictFrame =  ceil(dis / 6 / 0.02) + 12;
    if( 9000 - frameID > PredictFrame){
        return true;
    }else{
        return false;
    }
}


//根据当前机器人所处位置找到下一个买东西的工作台id
int NextBuyDes(const double& posx,const double& posy){


    //第零优先级：生产好的距离近的，且拿完有位置放的
    auto cmp0 = [&](WorkBase* a, WorkBase* b)->bool {
        return a->getDistance(posx,posy) > b->getDistance(posx,posy);
    };
    priority_queue<WorkBase*, vector<WorkBase*>, decltype(cmp0)> q0(cmp0);
    for(int i=0; i<K; i++){
        if(  (WorkList[i]->ProductIsOk || WorkList[i]->ProductToBeOK(posx,posy)) && WorkList[i]->ProductServerRobot == -1 &&
            NextSellDes(WorkList[i]->type,posx,posy) != -1 && 
            TimeEnoughToSell(i,NextSellDes(WorkList[i]->type,posx,posy))){
            q0.push(WorkList[i]);
        }
    }
    if (!q0.empty())  return q0.top()->id;


    // auto cmp_full_ok = [&](WorkBase* a, WorkBase* b)->bool {
    //     return a->type < b->type;
    // };
    // //第一优先级：材料格满且产品生产完的进队列，先消耗类型大的
    // priority_queue<WorkBase*, vector<WorkBase*>, decltype(cmp_full_ok)> q_full_ok(cmp_full_ok);

    // for(int i=0; i<K; i++){
    //     if(WorkList[i]->type > 3  && WorkList[i]->MaterialState == WorkList[i]->StateMask && 
    //          WorkList[i]->ProductIsOk  && WorkList[i]->ProductServerRobot == -1 &&
    //         NextSellDes(WorkList[i]->type,posx,posy) != -1 ){
    //         q_full_ok.push(WorkList[i]);
    //     }
    // }
    // if (!q_full_ok.empty())  return q_full_ok.top()->id;


    // //第二优先级距离小且已经生产好的高级产品(4,5,6,7)，先消耗距离小的
    // auto cmp_distance = [&](WorkBase* a, WorkBase* b)->bool {
    //     return a->getDistance(posx,posy) > b->getDistance(posx,posy);
    // };
    // priority_queue<WorkBase*, vector<WorkBase*>, decltype(cmp_distance)> q_ok(cmp_distance);
    // for(int i=0; i<K; i++){
    //     if( WorkList[i]->ProductIsOk == 1 && WorkList[i]->ProductServerRobot == -1 && WorkList[i]->type > 3
    //         && NextSellDes(WorkList[i]->type,posx,posy) != -1){
    //         q_ok.push(WorkList[i]);
    //     }
    // }
    // if (!q_ok.empty())  return q_ok.top()->id;


    auto cmp_123 = [&](WorkBase* a, WorkBase* b)->bool {
        int RemainCella = MasterialCells[a->type] - MasterialCellsUsed[a->type] + MasterialCellsBooked[a->type];
        int RemainCellb = MasterialCells[b->type] - MasterialCellsUsed[b->type] + MasterialCellsBooked[b->type];
        if(a->getDistance(posx,posy) > b->getDistance(posx,posy)) return true;
        else  if(a->getDistance(posx,posy) < b->getDistance(posx,posy)) return false;
        else if( RemainCella < RemainCellb ) return true;
        else if(RemainCella > RemainCellb) return false;
        else return false;
    };

    //上面都没找到，就去没人安排的1 2 3,去 1 2 3的原则是先去当前 场上空位多的 距离小的
    priority_queue<WorkBase*, vector<WorkBase*>, decltype(cmp_123)> q_base(cmp_123);
    for(int i=0; i<K; i++){
        //
        if( WorkList[i]->type <= 3  &&  WorkList[i]->ProductServerRobot == -1 &&
            NextSellDes(WorkList[i]->type,posx,posy) != -1 && 
            TimeEnoughToSell(i,NextSellDes(WorkList[i]->type,posx,posy))){
                q_base.push(WorkList[i]);
        }
    }
    if (!q_base.empty())  return q_base.top()->id;
    
    else return -1;
    //else return 
}




int FindNextDes(Robot* robot){
    if(robot->CarryType == 0){
        return NextBuyDes(robot->posx,robot->posy);
    }else{
        return NextSellDes(robot->CarryType,robot->posx,robot->posy);
    }
}




