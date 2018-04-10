#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	
#include "usmart.h"	 
#include "rtc.h" 
#include "QD.h" 
#include "pwm.h" 
#include "adc.h"
#include "control.h"
#include "timer.h"
#include "encoder.h"

float tt;

extern int x;
extern float kp,kd;


void up_moto(void)
{


		
			if(abs(pid.Sv-pid.Pv)<500)     //平衡状态监测到按键，则关闭电机
			{
			    TIM_SetCompare2(TIM3,3000);		
          TIM_SetCompare1(TIM1,0);	
			
			}
			else                                //否则起摆
			{                             
				TIM_SetCompare2(TIM3,1050);	 	  	
	      TIM_SetCompare1(TIM1,0);			
				delay_ms(550);
			 	TIM_SetCompare2(TIM3,3000);		
        TIM_SetCompare1(TIM1,1500);	
				delay_ms(120);			
			}
		

	}






int main(void)
 {	 
	u16 adcx;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();	    	 //延时函数初始化	  
	
	uart_init(115200);	 	//串口初始化为115200
	 
	LED_Init();			     //LED端口初始化
	 
	PID_Init();						//pid参数进行初始化	  
  
	Adc_Init(); 
	 
	 
	TIM1_PWM_Init(2999,0);		 //pwm周期3000ms 
    
  TIM3_PWM_Init(2999,0);		//pwm周期是3000ms		
	 	  
	TIM2_Int_Init(99,7199);		//10ms中断	
 	ENC_Init(); 
	 
	Init_Keyboard_Interrupt();			//按键

	LCD_Init();		
 
	POINT_COLOR=BLUE;//设置字体为蓝色	 
	LCD_ShowString(20,130,200,16,16,"ADC_CH0_VAL:");	

	
	while(1)
	{

		
		up_moto();
		 adcx=pid.Pv;		
					
		LCD_ShowxNum(116,130,adcx,4,16,0);//显示ADC的值

		if(pid.OUT>=0)
		{		
		LCD_ShowString(10,220,200,16,16,"PID.out: ");
		LCD_ShowxNum(80,220,pid.OUT,5,16,0X80);	
		}
		if(pid.OUT<0)
		{		
		LCD_ShowString(10,220,200,16,16,"PID.out:-");
		LCD_ShowxNum(80,220,0-pid.OUT,5,16,0X80);	
		}
		delay_ms(300);	
	
	
	} 
 }

