#ifndef _pid_H
#define _pid_H
#include "sys.h"

typedef struct
{
  float Angle_Sv;//�Ƕ�����ֵ
	
	float Encoder_Sv;//�ٶ�����ֵ
	
  float Angle_Pv;//�Ƕ�Ŀ��ֵ
	
	
	float Encoder_Pv1;//�ٶ�Ŀ��ֵ
	
	float Encoder_Pv2;//�ٶ�Ŀ��ֵ
	
	float Encoder_Pv;
	
	
	float Angle_out;
	
	float Encoder_out1;
	float Encoder_out2;
	
	
	float left_out;
	float right_out;
	float Kp;
	float T;//��������  ��������
	float Ti;
	float Td;
	
	float Ek;//����ƫ��
	float Ek_1;//�ϴ�ƫ��
	
	float Sek;//��ʷƫ�����
	float OUT0;
	
	float OUT;
	
	u16 pwm_T;
	
	u16 C_10ms;

}PID;


void PID_init(void);

extern PID pid;
#endif
