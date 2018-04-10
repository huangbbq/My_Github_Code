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
	
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为 115200
 	usmart_dev.init(72);		//初始化USMART		
 	LED_Init();		  			//初始化与LED连接的硬件接口
	LCD_Init();			   		//初始化LCD  
  Init_Keyboard_Interrupt();
	PID_Init(-14.28,2.61,78,79);
 	POINT_COLOR=RED;			//设置字体为红色 
	LCD_ShowString(30,70,200,16,16,"OV7670 TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/1/18");  
  	LCD_ShowString(30,50,200,16,16,"OV7670 Init...");	 
	while(OV7670_Init())//初始化OV7670
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
	TIM6_Int_Init(100,7199);			//10Khz计数频率,1秒钟中断									  
	EXTI8_Init();						//使能定时器捕获
	OV7670_Window_Set(12,176,160,160);	//设置窗口	
  OV7670_CS=0;			
	LCD_Clear(BLACK);
	
	
	
 	while(1)
	{	
		

	}
}









