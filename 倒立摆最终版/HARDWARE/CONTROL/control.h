#ifndef __control_H
#define __control_H	
#include "sys.h"

void pid_control(void);  
void PID_Init(void);
int velocity_mode1(void);
int balance_mode1(void);
int velocity_mode2(void);
int balance_mode2(void);
int velocity_mode3(void);
int balance_mode3(void);


typedef struct
{
u8 time;
float Sv;//目标值
float Pv;//传感器值
float Kp;//比例系数
float T;//pid计算周期
float Ti;//pid积分时间常数
float Td;//微分系数	
float Ek;	//本次偏差
float Ek_1;//上次偏差
float SEK1;//历史偏差和
float SEk2;
float OUT0;
float OUT;//当前输出控制值	
float Iout;
float Pout;
float Dout;
float enconter_out;
float angle_out;
}PID;

extern PID pid;//存放pid数据


#endif

