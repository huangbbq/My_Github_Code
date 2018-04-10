#ifndef _pid_H
#define _pid_H
#include "sys.h"

typedef struct
{
  float Angle_Sv;//角度设置值
	
	float Encoder_Sv;//速度设置值
	
  float Angle_Pv;//角度目标值
	
	
	float Encoder_Pv1;//速度目标值
	
	float Encoder_Pv2;//速度目标值
	
	float Encoder_Pv;
	
	
	float Angle_out;
	
	float Encoder_out1;
	float Encoder_out2;
	
	
	float left_out;
	float right_out;
	float Kp;
	float T;//计算周期  采样周期
	float Ti;
	float Td;
	
	float Ek;//本次偏差
	float Ek_1;//上次偏差
	
	float Sek;//历史偏差这和
	float OUT0;
	
	float OUT;
	
	u16 pwm_T;
	
	u16 C_10ms;

}PID;


void PID_init(void);

extern PID pid;
#endif
