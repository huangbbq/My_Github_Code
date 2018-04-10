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


		
			if(abs(pid.Sv-pid.Pv)<500)     //ƽ��״̬��⵽��������رյ��
			{
			    TIM_SetCompare2(TIM3,3000);		
          TIM_SetCompare1(TIM1,0);	
			
			}
			else                                //�������
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    	 //��ʱ������ʼ��	  
	
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	 
	LED_Init();			     //LED�˿ڳ�ʼ��
	 
	PID_Init();						//pid�������г�ʼ��	  
  
	Adc_Init(); 
	 
	 
	TIM1_PWM_Init(2999,0);		 //pwm����3000ms 
    
  TIM3_PWM_Init(2999,0);		//pwm������3000ms		
	 	  
	TIM2_Int_Init(99,7199);		//10ms�ж�	
 	ENC_Init(); 
	 
	Init_Keyboard_Interrupt();			//����

	LCD_Init();		
 
	POINT_COLOR=BLUE;//��������Ϊ��ɫ	 
	LCD_ShowString(20,130,200,16,16,"ADC_CH0_VAL:");	

	
	while(1)
	{

		
		up_moto();
		 adcx=pid.Pv;		
					
		LCD_ShowxNum(116,130,adcx,4,16,0);//��ʾADC��ֵ

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

