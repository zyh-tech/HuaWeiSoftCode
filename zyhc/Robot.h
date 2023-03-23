#pragma once

class Robot {
public:
    Robot(double posx, double posy) {
        this->id = id;
        this->posx = posx;
        this->posy = posy;
    }

    void update(int RobotWorkId, int CarryType, double TimeValue, double CollideValue, double PalStance,
        double LinearVel0, double LinearVel1, double Direction, double posx, double posy) {
        this->RobotWorkId = RobotWorkId;
        this->CarryType = CarryType;
        this->TimeValue = TimeValue;
        this->CollideValue = CollideValue;
        this->PalStance = PalStance;
        this->LinearVel[0] = LinearVel0;
        this->LinearVel[1] = LinearVel1;
        this->Direction = Direction;
        this->posx = posx;
        this->posy = posy;
    }
    int id;             //�����˵ı�ţ�1��2��3��4

                        //[0,����̨����-1] ����ʾĳ����̨���´� 0 ��ʼ��������˳�򶨡�
    int CarryType = 0;      //Я����Ʒ������[0-7]��0��ʾδЯ����Ʒ
    double posx;
    double posy;
    double TimeValue = 1;
    double CollideValue = 1;
    int RobotWorkId = -1;         //-1����ʾ��ǰû�д����κι���̨����
    int TargetWorkId = -1;          //�����˵�ǰ��Ŀ�ĵأ�һ������̨��id;
    double PalStance = 0;           //�����˽��ٶ�
    double LinearVel[2] = { 0,0 };     //���������ٶ�
    double Direction = 0;            //�����˳���
};