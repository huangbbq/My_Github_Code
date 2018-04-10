#ifndef __VARIABLE_H
#define __VARIABLE_H	
#include "stm32f10x.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"
#include "pwm.h"
#include "key.h"
#include <stdlib.h>
#include "adc.h"
#include "QD.h"
#include "control.h"
//±äÁ¿»ã×Ü
extern int speed,target,op1,op2,t1,t2,key_up;
extern u16 count_exp,angle0;
extern float kpa,kia,kda,kpv,kiv,kdv,tt; 


#endif

