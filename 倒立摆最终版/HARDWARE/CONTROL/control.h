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
float Sv;//Ŀ��ֵ
float Pv;//������ֵ
float Kp;//����ϵ��
float T;//pid��������
float Ti;//pid����ʱ�䳣��
float Td;//΢��ϵ��	
float Ek;	//����ƫ��
float Ek_1;//�ϴ�ƫ��
float SEK1;//��ʷƫ���
float SEk2;
float OUT0;
float OUT;//��ǰ�������ֵ	
float Iout;
float Pout;
float Dout;
float enconter_out;
float angle_out;
}PID;

extern PID pid;//���pid����


#endif

