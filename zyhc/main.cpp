
#include<memory.h> 

#include "GetCommond.h"
//#include "DestinationUpdate.h"
#include "UpdateState.h"
//#include "data.h"
#include "ReadMap.h"

#define _USE_MATH_DEFINES





int main() {
    //首先读入地图
    readMap();
    readUntilOK();
    //初始化四个机器人的目的地
    K = WorkList.size();
    for (int i = 0; i < RobotNum; i++) {
        int NextTarget = NextBuyDes(RobotList[i]->posx, RobotList[i]->posy);
        UpadateDes(RobotList[i],NextTarget,i);
        // RobotList[i]->TargetWorkId = NextTarget;                  //机器人目的地更新
        // WorkList[NextTarget]->ProductServerRobot = RobotId;     //与下一个买的地方进行买绑定
        // WorkList[NextTarget]->MaterialServerRobot[0] = RobotId;
    }

    puts("OK");
    fflush(stdout);

    //开始每帧的交互
    RobotId = -1;    //机器人序号
    WorkId = -1;     //工作台序号

    while (scanf("%d", &frameID) != EOF) {
        //读取判题器输入
        //cin >> frameID; 
        cin >> cash;    getchar();         //第一行是帧id和当前金额
        cin >> K;    getchar();                            //第二行是工作台数量
        //依次更新每个工作台的状态double TargetX = WorkList[robot->TargetWorkId]->posx;
        //MasterialCellsUsed = {0,0,0,0,0,0,0};
        memset(MasterialCellsUsed,sizeof(MasterialCellsUsed),0);
        for (int i = 0; i < K; i++) {
            cin >> WorkType;
            cin >> posx;      cin >> posy;
            cin >> RemainTime;
            cin >> MaterialState;
            cin >> ProductIsOk;
            WorkList[i]->update(WorkType, posx, posy, RemainTime, MaterialState, ProductIsOk);
            //统计当前空位材料格
            for(int i=1; i<8; i++){
                MasterialCellsUsed[i] +=  ( MaterialState>>i & 1);
            }
            getchar();
        }

        printf("%d\n", frameID);
        //接着读取四个机器人的状态
        for (int i = 0; i < RobotNum; i++) {
            cin >> RobotWorkId;
            cin >> CarryType;
            cin >> TimeValue;
            cin >> CollideValue;
            cin >> PalStance;
            cin >> LinearVel[0];  cin >> LinearVel[1];
            cin >> Direction;
            cin >> posx;      cin >> posy;
            getchar();

            RobotList[i]->update(RobotWorkId, CarryType, TimeValue, CollideValue, PalStance, LinearVel[0],
                LinearVel[1], Direction, posx, posy);
            
            if ( RobotList[i]->CarryType != 0 && RobotList[i]->TargetWorkId == RobotWorkId && 
                 (WorkList[RobotWorkId]->MaterialState>>CarryType & 1) == 0) {

                    Sell(RobotList[i],RobotWorkId,CarryType, i);
                    cout << "sell" << " " << i << endl; 
                    int NextTarget= FindNextDes(RobotList[i]);   //找到下一个目的地
                    if(NextTarget != -1){
                        UpadateDes(RobotList[i],NextTarget,i);
                    }
            }
    
            if( RobotList[i]->CarryType == 0 && RobotList[i]->TargetWorkId == RobotWorkId ){
                RobotList[i]->CarryType = WorkList[RobotWorkId]->type;
                if(FindNextDes(RobotList[i]) != -1){
                    Buy(RobotList[i],RobotWorkId,WorkList[RobotWorkId]->type, i);
                    cout << "buy" << " " << i << endl; 
                }
                int NextTarget= FindNextDes(RobotList[i]);   //找到下一个目的地
                if(NextTarget != -1){
                    UpadateDes(RobotList[i],NextTarget,i);
                }
            }
        }
        readUntilOK();


        // //看是否可以更换目的地
        // for (int i = 0; i < RobotNum; i++) {
        //     //释放掉原来的任务
        //     if(RobotList[i]->CarryType == 0){
        //         WorkList[RobotList[i]->TargetWorkId]->ProductServerRobot = -1;
        //     }else{
        //         WorkList[RobotList[i]->TargetWorkId]->MaterialServerRobot[RobotList[i]->CarryType] = -1;
        //     }

        //     int NextTarget= FindNextDes(RobotList[i]);   //找到下一个目的地
        //     if(NextTarget != -1){
        //         UpadateDes(RobotList[i],NextTarget,i);
        //     }
        // }


        for (int i = 0; i < RobotNum; i++) {
            OriginCommond[i] = GenCommond(RobotList[i]);
        }
        CollisionAvoid(OriginCommond);
        for (int i = 0; i < RobotNum; i++) {
            cout << "rotate" << " " << i << " " << OriginCommond[i][0] << endl;
            cout << "forward" << " " << i << " " << OriginCommond[i][1] << endl;
        }
        printf("OK\n");
        fflush(stdout);
    }
    return 0;
}




