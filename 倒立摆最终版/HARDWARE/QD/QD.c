#include "stm32f10x.h"
#include "QD.h"
#include "bianliang.h"
#include "pwm.h"
#include "sys.h"
#include "control.h"
#include "delay.h"
#include "timer.h"

extern u8 sign_flag;


void QD_Init_1(void)
{
	 float pwm1,pwm2;  
			 	 
    if (pid.OUT>0)
           {            
             	pwm1=pid.OUT;	   
	            TIM_SetCompare2(TIM3,pwm1);	 	  	//TIM3低电平
	            TIM_SetCompare1(TIM1,3000);				//低
          
            }
        else if(pid.OUT<0)
            {	
             	pwm2=-pid.OUT;          
              pwm2=3000-pwm2;
	            TIM_SetCompare2(TIM3,0);		
            	TIM_SetCompare1(TIM1,pwm2);	
						
         
            }
     
        else
           {
              TIM_SetCompare2(TIM3,3000);		
            	TIM_SetCompare1(TIM1,0);	

           }    

}

void QD_Init_2(void)
{
	 float pwm1,pwm2;  
			 	 
    if (pid.OUT>0)
           {            
             	pwm1=pid.OUT;	   
	            TIM_SetCompare2(TIM3,pwm1);	 	  	//TIM3低电平
	            TIM_SetCompare1(TIM1,3000);				//低
          
            }
    if (pid.OUT<0)
            {	
             	pwm2=-pid.OUT;          
              pwm2=3000-pwm2;
	            TIM_SetCompare2(TIM3,0);		
            	TIM_SetCompare1(TIM1,pwm2);	
						
         
            }   

}

void QD_Init_3(void)
{
	 float pwm1,pwm2;  
			 	 
    if (pid.OUT>0)
           {            
             	pwm1=pid.OUT;	   
	            TIM_SetCompare2(TIM3,pwm1);	 	  	//TIM3低电平
	            TIM_SetCompare1(TIM1,3000);				//低
          
            }
        else if(pid.OUT<0)
            {	
             	pwm2=-pid.OUT;          
              pwm2=3000-pwm2;
	            TIM_SetCompare2(TIM3,0);		
            	TIM_SetCompare1(TIM1,pwm2);	
						
         
            }
     
        else
           {
              up_moto3();

           }    

}

