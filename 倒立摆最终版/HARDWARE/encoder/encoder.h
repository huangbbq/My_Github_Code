#ifndef _encoder__H
#define _encoder__H
#include "sys.h"


#define count TIM4->CNT   //��ȡ����ֵ���ȼ���λ��
void ENC_Init(void);
int Read_Encoder(void);
#endif
