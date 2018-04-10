#include "stm32f10x.h"
#include "usart.h"
#include "usart3.h"
#include "encoder.h"
#include "motor.h"
#include "timer.h"
#include "pid.h"

extern float angle[3],speed[3],accel[3],temperature;//角度,角速度，角加速度,温度

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  PID_init();//PID参数初始化
  LED_Init();


  uart_init(115200);
	

	
	Encoder_Init_TIM2();//编码器1初始化
	Encoder_Init_TIM4();//编码器2初始化
	usart3_init();//串口3读角度值
	MiniBalance_Motor_Init();//电机IO初始化
	MiniBalance_PWM_Init(4999,0);//PWM初始化
	TIM3_Int_Init(99,719); //1ms	

  while(1)
    {
//			 int z,x;
//       Read_angle();
//			 z=Read_Encoder(2);
//			 x=Read_Encoder(4);
//			 //printf("左电机：%d   右电机：%d\r\n",z,x);
			
			
     }

}
