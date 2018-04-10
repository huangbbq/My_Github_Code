#ifndef __QD_H
#define __QD_H	 
#include "sys.h"

#define QD1 PAout(7)// PA6
#define QD2 PAout(8)// PB6

//extern float pwm1,pwm2;

void QD_Init_1(void);
void QD_Init_2(void);
void QD_Init_3(void);

#endif

