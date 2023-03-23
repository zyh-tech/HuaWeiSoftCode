//该文件用于当机器人到达工作台时更新状态，包括机器人和对应的工作台

#pragma once

#include "NextDestination.h"

using namespace std;

bool Sell(Robot *robot,int& RobotWorkId, int& CarryType, int& RobotId){
    // int NextTarget= NextBuyDes(robot->posx, robot->posy);   //找到下一个买的地方

    // if(NextTarget != -1){

    WorkList[RobotWorkId]->MaterialState |= (1 << CarryType);   //工作台材料状态更新
    MasterialCellsUsed[CarryType] += 1;                         //场上使用的材料格子更新
    MasterialCellsBooked[CarryType] -= 1;                       //场上预定的材料格子更新
    robot->CarryType = 0;                                       //卸下机器人当前携带货物
    WorkList[RobotWorkId]->MaterialServerRobot[CarryType] = -1;   //取消与工作台材料格卖绑定

        // robot->TargetWorkId = NextTarget;                      //机器人目的地更新
        // WorkList[NextTarget]->ProductServerRobot = RobotId;     //与下一个买的地方进行买绑定

    //     return true;
    // }
    // else return false;
}



bool Buy(Robot *robot,int& RobotWorkId, int& ProductType, int& RobotId){
    // int NextTarget= NextSellDes(ProductType, robot->posx, robot->posy);   //找到下一个卖的地方

    // if(NextTarget != -1){

    WorkList[RobotWorkId]->ProductIsOk = 0;                     //工作台产品状态更新
    robot->CarryType = ProductType;                         //机器人携带新货物
    WorkList[RobotWorkId]->ProductServerRobot = -1;         //机器人取消与工作台材料格买绑定

    //     robot->TargetWorkId = NextTarget;                                  //机器人目的地更新
    //     WorkList[NextTarget]->MaterialServerRobot[ProductType] = RobotId;  //与下一个卖的地方进行卖绑定
    //     MasterialCellsBooked[ProductType] += 1;                         //场上已经被预定的格子更新

    //     return true;
    // }
    // else return false;
}




void UpadateDes(Robot* robot,int NextTarget,int RobotId){
    if(robot->CarryType == 0){
        robot->TargetWorkId = NextTarget;                                   //机器人目的地更新
        //if(WorkList[NextTarget]->type > 3){
        WorkList[NextTarget]->ProductServerRobot = RobotId; 
        //}        //与下一个买的地方进行买绑定
    }else{
         robot->TargetWorkId = NextTarget;                                  //机器人目的地更新
         WorkList[NextTarget]->MaterialServerRobot[robot->CarryType] = RobotId;  //与下一个卖的地方进行卖绑定
         MasterialCellsBooked[robot->CarryType] += 1;                            //场上已经被预定的格子更新
    }
}



// bool CanBuy(int type){
//     return MasterialCells[type] - MasterialCellsUsed[type] - MasterialCellsBooked[type] > 0;
// }