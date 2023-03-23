
#pragma once

#include <queue>
#include "data.h"
using namespace std;


//快速统计一个二进制数字中1的个数
int lowbit(int x) {
    return x & -x;
}


//根据机器人当前携带类型和当前位置选取目的地的函数
int DestinationUpdate(const int& CarryType, double& posx, double posy) {
    //排序规则如下
    //首先对应材料有空位置的工作台才有资格进入优先队列参与排序
    //物品生产完成的优先于物品没有生产完的                -------这个某些地图有用
    //当前缺少材料少的优先
    //物品都没生产完，则距离近的优先
    //物品都没生产完且距离相同则剩余生产时间短的优
    //若最后队列为空，则去找9类型工作台卖掉


    //这里可以优化为当前工作台的当前材料台是否被当作目标
    //比较方式中应该再加入当前工作台没有被任意机器人当作目标


    auto cmp = [&](WorkBase* a, WorkBase* b)->bool {
        if (a->ProductIsOk < b->ProductIsOk) return true;
        else if (a->ProductIsOk > b->ProductIsOk) return false;
        // if(MasterialCells[a->type] - MasterialCellsUsed[a->type] < MasterialCells[b->type] - MasterialCellsUsed[b->type])
        //     return true;
        // else if(MasterialCells[a->type] - MasterialCellsUsed[a->type] > MasterialCells[b->type] - MasterialCellsUsed[b->type])
        //     return false;
        // else if  (lowbit(a->MaterialState) < lowbit(b->MaterialState)) return true;
        // else if  (lowbit(a->MaterialState) > lowbit(b->MaterialState)) return false;
        else if (a->getDistance(posx, posy) > b->getDistance(posx, posy)) return true;
        else if (a->getDistance(posx, posy) < b->getDistance(posx, posy)) return false;
        else if (a->RemainTime > b->RemainTime) return true;
        else return false;
    };
    priority_queue<WorkBase*, vector<WorkBase*>, decltype(cmp)> q(cmp);
    while (!q.empty()) q.pop();


    switch (CarryType) {
    case 0: {   //当携带0的时候，需要去 1 2 3
            //这里//优先取材料满，且有成品，且成品可以有空位卖出
            auto cmp0 = [&](WorkBase* a, WorkBase* b)->bool {
                return a->type < b->type;
            };
            priority_queue<WorkBase*, vector<WorkBase*>, decltype(cmp0)> q0(cmp0);
            for (int i = 0; i < K; i++) {
                //WorkList[i]->MaterialState == WorkList[i]->StateMask &&
                if (    WorkList[i]->ProductIsOk && WorkList[i]->ServerRobot == -1 && 
                       MasterialCells[WorkList[i]->type] > MasterialCellsUsed[WorkList[i]->type] ) {
                            q0.push(WorkList[i]);
                }
            }
            if (!q0.empty())  return q0.top()->id;

            // int TargetType;
            // if((MasterialCells[1] - MasterialCellsUsed[1]) > (MasterialCells[2] - MasterialCellsUsed[2])){
            //     if((MasterialCells[1] - MasterialCellsUsed[1]) > (MasterialCells[3] - MasterialCellsUsed[3]))
            //         TargetType = 1;
            //     else TargetType = 3;
            // }else{
            //     if((MasterialCells[2] - MasterialCellsUsed[2]) > (MasterialCells[3] - MasterialCellsUsed[3]))
            //         TargetType = 2;
            //     else TargetType = 3;
            // }
            // if (WorkList[i]->ServerRobot == -1 && WorkList[i]->type == TargetType) {
            //     q.push(WorkList[i]);
            // }
            for (int i = 0; i < K; i++) {
                if ( WorkList[i]->ServerRobot == -1 &&
                    (WorkList[i]->type == 1 || WorkList[i]->type == 2|| WorkList[i]->type == 3)) {
                    q.push(WorkList[i]);
                }
            }
            break;
    }
    case 1: {   //当携带1的时候，需要去 4 5类型, 
        // int TargetType = (MasterialCells[4] - MasterialCellsUsed[4]) > (MasterialCells[5] - MasterialCellsUsed[5]) ? 4 : 5;
        // for (int i = 0; i < K; i++) {
        //     if (WorkList[i]->ServerRobot == -1 && WorkList[i]->type == TargetType ) {
        //         if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
        //     }
        // }
        for (int i = 0; i < K; i++) {
            if ((WorkList[i]->type == 4 || WorkList[i]->type == 5 ) && WorkList[i]->MaterialServerRobot[1] == -1 ) {
                if (( (WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
            }
        }
        break;
    }
    case 2: {   //当携带2的时候，需要去 4 6类型
        // int TargetType = (MasterialCells[4] - MasterialCellsUsed[4]) > (MasterialCells[6] - MasterialCellsUsed[6]) ? 4 : 6;
        // for (int i = 0; i < K; i++) {
        //     if (WorkList[i]->ServerRobot == -1 &&  WorkList[i]->type == TargetType ) {
        //         if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
        //     }
        // }
        for (int i = 0; i < K; i++) {
            if ( (WorkList[i]->type == 4 || WorkList[i]->type == 6 ) && WorkList[i]->MaterialServerRobot[2] == -1) {
                if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
            }
        }
        break;
    }
    case 3: {   //当携带3的时候，需要去 5 6类型
        // int TargetType = (MasterialCells[5] - MasterialCellsUsed[5]) > (MasterialCells[6] - MasterialCellsUsed[6]) ? 5 : 6;
        // for (int i = 0; i < K; i++) {
        //     if (WorkList[i]->ServerRobot == -1 && WorkList[i]->type == TargetType) {
        //         if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
        //     }
        // }
        for (int i = 0; i < K; i++) {
            if ((WorkList[i]->type == 5 || WorkList[i]->type == 6 ) && WorkList[i]->MaterialServerRobot[3] == -1) {
                if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
            }
        }
        break;
    }
    case 4: {  //当携带4的时候，需要去7类型, 
        for (int i = 0; i < K; i++) {
            if ( WorkList[i]->type == 7 && WorkList[i]->MaterialServerRobot[4] == -1  ) {
                if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
            }
        }
        break;
    }
    case 5: {   //当携带5的时候，需要去7类型
        for (int i = 0; i < K; i++) {
            if (WorkList[i]->type == 7 &&  WorkList[i]->MaterialServerRobot[5] == -1 ) {
                if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
            }
        }
        break;
    }
    case 6: {   //当携带6的时候，需要去7类型
        for (int i = 0; i < K; i++) {
            if (WorkList[i]->type == 7 &&  WorkList[i]->MaterialServerRobot[6] == -1) {
                if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
            }
        }
        break;
    }
    case 7: {   //当携带7的时候，需要去8、9类型, 
        for (int i = 0; i < K; i++) {
            if ( (WorkList[i]->type == 8 || WorkList[i]->type == 9)) {
                q.push(WorkList[i]);
            }
        }
        break;
    }
    case 8: {   //这个情况比较特殊，当机器人身上载的货物当前没有空位可放的时候，就去找9卖掉
            //寻找距离最近的9
        auto cmp8 = [&](WorkBase* a, WorkBase* b)->bool {
            return a->getDistance(posx, posy) < b->getDistance(posx, posy);
        };
        priority_queue<WorkBase*, vector<WorkBase*>, decltype(cmp8)> q8(cmp8);
        for (int i = 0; i < K; i++) {
            if (WorkList[i]->type == 9) {
                q8.push(WorkList[i]);
            }
        }
        if (!q8.empty())  return q8.top()->id;
        else return -1;   //返回-1表示没地方可去，摧毁物品
    }
    //default:  cout << "DestinationUpdate函数输入类型错误" << endl;
    }
    if (!q.empty())  return q.top()->id;
    else return DestinationUpdate(8, posx, posy);
   // return q.top()->id;
}


// //快速统计一个二进制数字中1的个数
// int lowbit(int x) {
//     return x & -x;
// }


// //根据机器人当前携带类型和当前位置选取目的地的函数
// int DestinationUpdate(const int& CarryType, double& posx, double posy) {
//     //排序规则如下
//     //首先对应材料有空位置的工作台才有资格进入优先队列参与排序
//     //物品生产完成的优先于物品没有生产完的                -------这个某些地图有用
//     //当前缺少材料少的优先
//     //物品都没生产完，则距离近的优先
//     //物品都没生产完且距离相同则剩余生产时间短的优
//     //若最后队列为空，则去找9类型工作台卖掉


//     //这里可以优化为当前工作台的当前材料台是否被当作目标
//     //比较方式中应该再加入当前工作台没有被任意机器人当作目标

//     auto cmp = [&](WorkBase* a, WorkBase* b)->bool {
//         if (a->ProductIsOk < b->ProductIsOk) return true;
//         else if (a->ProductIsOk > b->ProductIsOk) return false;
//         // if(MasterialCells[a->type] - MasterialCellsUsed[a->type] < MasterialCells[b->type] - MasterialCellsUsed[b->type])
//         //     return true;
//         // else if(MasterialCells[a->type] - MasterialCellsUsed[a->type] > MasterialCells[b->type] - MasterialCellsUsed[b->type])
//         //     return false;
//         // else if  (lowbit(a->MaterialState) < lowbit(b->MaterialState)) return true;
//         // else if  (lowbit(a->MaterialState) > lowbit(b->MaterialState)) return false;
//         else if (a->getDistance(posx, posy) > b->getDistance(posx, posy)) return true;
//         else if (a->getDistance(posx, posy) < b->getDistance(posx, posy)) return false;
//         else if (a->RemainTime > b->RemainTime) return true;
//         else return false;
//     };
//     priority_queue<WorkBase*, vector<WorkBase*>, decltype(cmp)> q(cmp);
//     while (!q.empty()) q.pop();


//     switch (CarryType) {
//     case 0: {   //当携带0的时候，需要去 1 2 3
//             //这里//优先取材料满，且有成品，且成品可以有空位卖出
//             auto cmp0 = [&](WorkBase* a, WorkBase* b)->bool {
//                 return a->type > b->type;
//             };
//             priority_queue<WorkBase*, vector<WorkBase*>, decltype(cmp0)> q0(cmp0);
//             for (int i = 0; i < K; i++) {
//                 //WorkList[i]->MaterialState == WorkList[i]->StateMask &&
//                 //MasterialCells[WorkList[i]->type] > MasterialCellsUsed[WorkList[i]->type]
//                 if (  WorkList[i]->ProductIsOk && WorkList[i]->ServerRobot == -1  &&
//                     MasterialCells[WorkList[i]->type] > MasterialCellsUsed[WorkList[i]->type]) {
//                         q0.push(WorkList[i]);
//                 }
//             }
//             if (!q0.empty())  return q0.top()->id; break;
            

//             for (int i = 0; i < K; i++) {
//                 if (WorkList[i]->ServerRobot == -1 && 
//                     (WorkList[i]->type == 1 || WorkList[i]->type == 2|| WorkList[i]->type == 3)) {
//                     q.push(WorkList[i]);
//                 }
//             }
//             break;
//     }
//     case 1: {   //当携带1的时候，需要去 4 5类型, 
//         // int TargetType = (MasterialCells[4] - MasterialCellsUsed[4]) > (MasterialCells[5] - MasterialCellsUsed[5]) ? 4 : 5;
//         // for (int i = 0; i < K; i++) {
//         //     if (WorkList[i]->ServerRobot == -1 && WorkList[i]->type == TargetType ) {
//         //         if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
//         //     }
//         // }
//         for (int i = 0; i < K; i++) {
//             if ((WorkList[i]->type == 4 || WorkList[i]->type == 5 ) && WorkList[i]->MaterialServerRobot[1] == -1 ) {
//                 if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
//             }
//         }
//         break;
//     }
//     case 2: {   //当携带2的时候，需要去 4 6类型
//         // int TargetType = (MasterialCells[4] - MasterialCellsUsed[4]) > (MasterialCells[6] - MasterialCellsUsed[6]) ? 4 : 6;
//         // for (int i = 0; i < K; i++) {
//         //     if (WorkList[i]->ServerRobot == -1 &&  WorkList[i]->type == TargetType ) {
//         //         if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
//         //     }
//         // }
//         for (int i = 0; i < K; i++) {
//             if ( (WorkList[i]->type == 4 || WorkList[i]->type == 6 ) && WorkList[i]->MaterialServerRobot[2] == -1) {
//                 if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
//             }
//         }
//         break;
//     }
//     case 3: {   //当携带3的时候，需要去 5 6类型
//         // int TargetType = (MasterialCells[5] - MasterialCellsUsed[5]) > (MasterialCells[6] - MasterialCellsUsed[6]) ? 5 : 6;
//         // for (int i = 0; i < K; i++) {
//         //     if (WorkList[i]->ServerRobot == -1 && WorkList[i]->type == TargetType) {
//         //         if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
//         //     }
//         // }
//         for (int i = 0; i < K; i++) {
//             if ((WorkList[i]->type == 5 || WorkList[i]->type == 6 ) && WorkList[i]->MaterialServerRobot[3] == -1) {
//                 if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
//             }
//         }
//         break;
//     }
//     case 4: {  //当携带4的时候，需要去7类型, 
//         for (int i = 0; i < K; i++) {
//             if ( WorkList[i]->type == 7 && WorkList[i]->MaterialServerRobot[4] == -1  ) {
//                 if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
//             }
//         }
//         break;
//     }
//     case 5: {   //当携带5的时候，需要去7类型
//         for (int i = 0; i < K; i++) {
//             if (WorkList[i]->type == 7 &&  WorkList[i]->MaterialServerRobot[5] == -1 ) {
//                 if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
//             }
//         }
//         break;
//     }
//     case 6: {   //当携带6的时候，需要去7类型
//         for (int i = 0; i < K; i++) {
//             if (WorkList[i]->type == 7 &&  WorkList[i]->MaterialServerRobot[6] == -1) {
//                 if (((WorkList[i]->MaterialState >> CarryType) & 1) == 0) q.push(WorkList[i]);
//             }
//         }
//         break;
//     }
//     case 7: {   //当携带7的时候，需要去8、9类型, 
//         for (int i = 0; i < K; i++) {
//             if ( (WorkList[i]->type == 8 || WorkList[i]->type == 9)) {
//                 q.push(WorkList[i]);
//             }
//         }
//         break;
//     }
//     case 8: {   //这个情况比较特殊，当机器人身上载的货物当前没有空位可放的时候，就去找9卖掉
//             //寻找距离最近的9
//         auto cmp8 = [&](WorkBase* a, WorkBase* b)->bool {
//             return a->getDistance(posx, posy) < b->getDistance(posx, posy);
//         };
//         priority_queue<WorkBase*, vector<WorkBase*>, decltype(cmp8)> q8(cmp8);
//         for (int i = 0; i < K; i++) {
//             if (WorkList[i]->type == 9) {
//                 q8.push(WorkList[i]);
//             }
//         }
//         if (!q8.empty())  return q8.top()->id;
//         else return -1;   //返回-1表示没地方可去
//     }
//     //default:  cout << "DestinationUpdate函数输入类型错误" << endl;
//     }
//     if (!q.empty())  return q.top()->id;
//     else return DestinationUpdate(8, posx, posy);
//    // return q.top()->id;
// }
