#ifndef __PWM_H
#define __PWM_H
#include "sys.h"
 
#define pwm 	 TIM1->CCR1        //’ºø’±»÷µ

void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
void TIM5_PWM_Init(u16 arr,u16 psc);
#endif
