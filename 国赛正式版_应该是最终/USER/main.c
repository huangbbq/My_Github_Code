#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "ov7670.h"
#include "timer.h"
#include "exti.h"
#include "usmart.h"
#include "stdio.h"
#include "location.h"
#include "usart3.h"
#include "pid.h"
 


 int main(void)
 {	 
	
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ 115200
 	usmart_dev.init(72);		//��ʼ��USMART		
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   		//��ʼ��LCD  
  Init_Keyboard_Interrupt();
	PID_Init(-14.28,2.61,78,79);
 	POINT_COLOR=RED;			//��������Ϊ��ɫ 
	LCD_ShowString(30,70,200,16,16,"OV7670 TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/1/18");  
  	LCD_ShowString(30,50,200,16,16,"OV7670 Init...");	 
	while(OV7670_Init())//��ʼ��OV7670
	{
		LCD_ShowString(30,70,200,16,16,"OV7670 Error!!");
		delay_ms(200);
	  LCD_Fill(30,210,239,246,WHITE);
		delay_ms(200);
	}
 	LCD_ShowString(30,700,200,16,16,"OV7670 Init OK");
	delay_ms(1500);	 	   
	usart3_init();
	TIM3_PWM_Init(9999,143);
	TIM6_Int_Init(100,7199);			//10Khz����Ƶ��,1�����ж�									  
	EXTI8_Init();						//ʹ�ܶ�ʱ������
	OV7670_Window_Set(12,176,160,160);	//���ô���	
  OV7670_CS=0;			
	LCD_Clear(BLACK);
	
	
	
 	while(1)
	{	
		

	}
}









