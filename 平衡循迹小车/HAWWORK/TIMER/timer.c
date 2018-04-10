#include "timer.h"
#include <stdlib.h>
#include "pid.h"
#include "control.h"
#include "motor.h"
#include "usart.h"

extern float angle[3],speed[3],accel[3],temperature;//角度,角速度，角加速度,温度


void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
  
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx		
  	
}


void TIM3_IRQHandler(void)   //TIM3中断
{

  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{	     
		  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志
	     pid.T++;
     if(pid.T==10)
		 {
			 pid.left_out=Calc_angle_pid()-Calc_encoder1_pid();
			 pid.right_out=pid.left_out;
			 if(abs(pid.Angle_Pv-pid.Angle_Sv)>30)           
		     {	
             pid.left_out=0;
					   pid.right_out=0;
					   pid.Sek=0;
		      }	
			 motor_qd();
			 pid.T=0;
			 //printf("左电机：%f      右电机：%f\r\n",pid.left_out,pid.right_out);
		 }

	   
	
	}

}

