#pragma once

#include<math.h>
#include "data.h"
using namespace std;


 vector<vector<double>> OriginCommond(RobotNum,vector<double>(2));                  //存放碰撞检测前的下一帧移动指令
 //vector<vector<double>> CollisionAvoidCommond(RobotNum,vector<double>(2));          //存放碰撞检测后的下一帧移动指令


vector<double> GenCommond(Robot* robot) {
    double Direction = robot->Direction;    //A
    double RobotX = robot->posx;
    double RobotY = robot->posy;
    double TargetX = WorkList[robot->TargetWorkId]->posx;
    double TargetY = WorkList[robot->TargetWorkId]->posy;
    double Angle = 0;                       //B

    double rotate=0, forward=6;
    double Angle_X = 0;
    double dy = TargetY - RobotY;
    double dx = TargetX - RobotX;
    int yflag = 1;
    int xyflag = 1;
    double Distance = 0;

    yflag = dy < 0 ? -1 : 1;
    xyflag = (dx * dy) < 0 ? -1 : 1;

    //计算机器人当前位置与目标位置连线与x正方向夹角
    if (abs(dy - 0) < 1e-8) {
        if (dx < 0)  Angle = -M_PI;
        else Angle = 0;
    }
    else {
        Distance = sqrt(pow(dx, 2) + pow(dy, 2));
        Angle = yflag * M_PI_2 - xyflag * acos(abs(dy) / Distance);
    }

    //计算机器人转动速度rotate
    if (abs(Angle - Direction ) < 1e-8) {
        rotate = 0;
    }
    else {
        if ((Angle >= 0 && Direction >= 0) || (Angle <= 0 && Direction <= 0) ){
            rotate= (Angle - Direction);
        }
        else if (Angle >= 0 && Direction <= 0) {
            if ((Angle - Direction) >= M_PI) {
                rotate = -(2 * M_PI - (Angle - Direction));
            }
            else {
                rotate =(Angle - Direction);
            }
        }
        else
        {
            if (Angle <= 0 && Direction>=0) {
                if ((Angle - Direction) <= -M_PI) {
                    rotate = (2 * M_PI + (Angle - Direction));
                }
                else {
                    rotate = (Angle - Direction);
                }
            }
        }
        //rotate = abs(Angle - Direction) > M_PI ? M_PI : 4.3* (Angle - Direction);    //距离越大角度调整越小
    }

    rotate = 3.5 * rotate;    //角度差越大角度调整越大
    //计算forward
    forward = 6 * pow(M_E, -abs(rotate)) * Distance * Distance; //*Distance* Distance
    //forward = max(forward, 0.4);
    return{ rotate,forward };
}



int PredictFrame = 25;                //预测帧数，假定机器人在该帧数内都是直线运动

void CollisionAvoid(vector<vector<double>>& OriginCommond){
     vector<vector<double>> FiturePos(RobotNum,vector<double>(2));               
     double curx = 0, cury = 0, curDirection = 0, curVx = 0, curVy = 0, FutureDis = 0;

        //计算PredictFrame后机器人的预测位置
     for(int i=0; i<RobotNum; i++){
        curx =  RobotList[i]->posx;
        cury =  RobotList[i]->posy;
        curDirection = RobotList[i]->Direction;
        curVx = RobotList[i]->LinearVel[0];
        curVy = RobotList[i]->LinearVel[1];
        FiturePos[i][0] = curx + curVx * PredictFrame * TimeOfFrame;
        FiturePos[i][1] = cury + curVy * PredictFrame * TimeOfFrame;
     }

        //计算PredictFrame后机器人之间的距离，根据距离是否小于阈值来决定是否需要调整当前指令
    int iType = 0, jType = 0;
     for(int i=0; i<RobotNum; i++){
        iType = RobotList[i]->CarryType;
        for(int j=0; j<RobotNum; j++){
            //计算PredictFrame后机器人之间的距离
            FutureDis = sqrt( pow((FiturePos[i][0]-FiturePos[j][0]),2) + pow((FiturePos[i][1]-FiturePos[j][1]),2) );
            jType = RobotList[j]->CarryType;

            //二者都没携带材料，阈值为0.9,一个带一个没带，阈值为0.98，两个都没带，阈值为1.06
            if(  (iType + jType == 0  &&  FutureDis <= 0.9 ) ||
                (iType + jType > 0  && iType * jType == 0  &&  FutureDis <= 0.98) ||
                (iType + jType > 0  && iType * jType > 0  &&  FutureDis <= 1.06) ){
                    if( (RobotList[i]->Direction * RobotList[j]->Direction) <= 0 ){
                        OriginCommond[i][0] += M_PI;
                        OriginCommond[j][0] += M_PI;
                        // OriginCommond[i][1] = 8 * pow(M_E, -abs(OriginCommond[i][0]));
                        // OriginCommond[j][1] = 8 * pow(M_E, -abs(OriginCommond[j][0]));
                    }else{
                        OriginCommond[i][0] += M_PI;
                        OriginCommond[j][0] -= M_PI;
                        // OriginCommond[i][1] = 8 * pow(M_E, -abs(OriginCommond[i][0]));
                        // OriginCommond[j][1] = 8 * pow(M_E, -abs(OriginCommond[j][0]));
                    }
            }
        }
     }
}






// vector<double> GenCommond(Robot* robot) {
//     double Direction = robot->Direction;    //A
//     double RobotX = robot->posx;
//     double RobotY = robot->posy;
//     double TargetX = WorkList[robot->TargetWorkId]->posx;
//     double TargetY = WorkList[robot->TargetWorkId]->posy;
//     double Angle = 0;                       //B

//     double rotate, forward;
//     double Angle_X = 0;
//     double dy = TargetY - RobotY;
//     double dx = TargetX - RobotX;
//     //int yflag = 1;
//     //int xyflag = 1;

//     int yflag = dy < 0 ? -1 : 1;
//     int xyflag = (dx * dy) < 0 ? -1 : 1;
//     int DirectionFlag = DirectionFlag  < 0 ? -1 : 1;

//     //计算机器人当前位置与目标位置连线与x正方向夹角
//     if (abs(dy - 0) < 1e-8) {
//         if (dx < 0)  Angle = -M_PI;
//         else  Angle = 0;
//     }else {
//         double Distance = sqrt(pow(dx, 2) + pow(dy, 2));
//         Angle = yflag * M_PI_2 - xyflag * acos(abs(dy) / Distance);
//     }

//     //计算机器人转动速度rotate
//     if( abs(Direction)+abs(Angle) > M_PI ) {
//         rotate = abs( abs(Angle)-abs(Direction) ) * DirectionFlag;
//     }else{
//         rotate = Angle - Direction;
//     }

//     if(rotate > M_PI) rotate = M_PI;
//     if(rotate < -M_PI) rotate = -M_PI;
//    // rotate = abs(Angle - Direction)>M_PI?M_PI:Angle-Direction;
//     //计算forward
//     forward = 6 * pow(M_E, -abs(rotate));
//     return{ rotate,forward };
// }