#include <stdlib.h>
#include "timer.h"
#include "led.h"
#include "control.h"
#include "adc.h"
#include "QD.h"
#include "usart.h"
#include "encoder.h"
#include "delay.h"

extern u8 mode_flag1,mode_flag2,mode_flag3;

void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
  
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM2, ENABLE);  //使能TIMx		
  	
}



void TIM6_Init(void)
{
    
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	 
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; /*它的抢占优先级可以尽量设置低点*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
		 
  TIM_TimeBaseStructure.TIM_Period = 200- 1;       //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;	    //
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  TIM_ClearITPendingBit(TIM6, TIM_IT_Update); 
  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); //溢出为中断源
  TIM_ARRPreloadConfig(TIM6, ENABLE);			 // 使能TIM6重载寄存器ARR

  /* TIM6 enable counter */
   TIM_Cmd(TIM6, ENABLE);                   //使能定时器6	
}





//定时器2中断服务程序
void TIM2_IRQHandler(void)   //TIM2中断
{		
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	  {			
				
			if(mode_flag1==1)
					 {
						   
   						 pid.OUT=balance_mode1()-velocity_mode1(); 
						 
						 	printf("X%d\r\n",(int)pid.OUT);
	             while(RESET==USART_GetFlagStatus(USART1,USART_FLAG_TC));
			
	         	         if(pid.OUT>3000)		//pwm周期  主函数中设置3
		                   {
			                      pid.OUT=3000;	
		                   }
		                 else if(pid.OUT<-3000)
		                   {
			                      pid.OUT=-3000;
		                   }	
		                if(abs(pid.Pv-pid.Sv)>400)           
		                   {	
			                      pid.OUT=0;
														pid.SEK1=0;
														pid.SEk2=0;
		                   }						
        			
														QD_Init_1(); 
											    
				  }	
					 
			
			 else if(mode_flag2==1)
					 {
						pid.OUT=balance_mode2()-velocity_mode2(); 
			
	         	if(pid.OUT>3000)		//pwm周期  主函数中设置3
		          {
			            pid.OUT=3000;	
		           }
		        else if(pid.OUT<-3000)
		          {
			            pid.OUT=-3000;
		           }	
		         if(abs(pid.Pv-pid.Sv)>600)           
		           {	
			              pid.OUT=0;
								    pid.SEK1=0;
								    pid.SEk2=0;
		           }						
        			
							 QD_Init_2();
       						 
					}		
					 
					 
				else if(mode_flag3==1)
					 {
						pid.OUT=balance_mode3()-velocity_mode3(); 
			
	         	if(pid.OUT>3000)		//pwm周期  主函数中设置3
		          {
			            pid.OUT=3000;	
		           }
		        else if(pid.OUT<-3000)
		          {
			            pid.OUT=-3000;
		           }	
		         if(abs(pid.Pv-pid.Sv)>400)           
		           {	
			              pid.OUT=0;
								    pid.SEK1=0;
								    pid.SEk2=0;
		           }						
        			
							 QD_Init_3();
       						 
					}	
					 
        else 		
			   {
			 
			            TIM_SetCompare2(TIM3,3000);		
            	    TIM_SetCompare1(TIM1,0);
			    }				 

					
		}		
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志 		
}


 void up_moto2(void)
{



				TIM_SetCompare2(TIM3,1050);	 	  	
	      TIM_SetCompare1(TIM1,0);			
				delay_ms(550);
			 	TIM_SetCompare2(TIM3,3000);		
        TIM_SetCompare1(TIM1,1500);	
				delay_ms(120);		
			
		

}

 void up_moto3(void)
{



				TIM_SetCompare2(TIM3,950);	 	  	
	      TIM_SetCompare1(TIM1,0);			
				delay_ms(400);
			 	TIM_SetCompare2(TIM3,3000);		
        TIM_SetCompare1(TIM1,1500);	
				delay_ms(350);		
			
		

}








