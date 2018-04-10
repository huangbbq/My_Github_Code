#include "encoder.h"
#include "usart.h"

extern int countext,revext,rev90;
extern s32 ext_anglespeed,motorspeed,ext_aspeed,motor_aspeed,motor_angle;
extern float ext_angle;
extern u8 motorflag,motorcount,speedflag1,PID_Flag,Rx_Flag;


//stm32中的编码器模式，需要用到两个定时器通道，此处用的定时器3
void ENC_Init(void)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_ICInitTypeDef TIM_ICInitStructure;

/* Encoder unit connected to TIM3, 4X mode */    
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

/* TIM3 clock source enable */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
/* Enable GPIOB, clock */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

GPIO_StructInit(&GPIO_InitStructure);
/* Configure PA.06,07 as encoder input */
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOB, &GPIO_InitStructure);

/* Enable the TIM3 Update Interrupt */
NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

/* Timer configuration in Encoder mode */
TIM_DeInit(TIM4);
TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // No prescaling 
TIM_TimeBaseStructure.TIM_Period = 0XFFFF; 
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
TIM_ICStructInit(&TIM_ICInitStructure);
TIM_ICInitStructure.TIM_ICFilter = 10;
TIM_ICInit(TIM4, &TIM_ICInitStructure);

// Clear all pending interrupts
TIM_ClearFlag(TIM4, TIM_FLAG_Update);
TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
//初始化位置
//TIM4->CNT = 32000;
// ENC_Clear_Speed_Buffer();
TIM_Cmd(TIM4, ENABLE); 
}

int Read_Encoder()
{
    int Encoder_TIM;    
		Encoder_TIM= TIM_GetCounter(TIM4);	
	  TIM4 ->CNT=200;	
		return Encoder_TIM;
}

void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//清除中断标志位 	    
}






