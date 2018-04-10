#ifndef _encoder__H
#define _encoder__H
#include "sys.h"


#define count TIM4->CNT   //读取计数值，等价于位置
void ENC_Init(void);
int Read_Encoder(void);
#endif
