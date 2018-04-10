#include "pwm2.h"
/*
 * 函数名：TIM3_GPIO_Config
 * 描述  ：配置TIM3输出的PWM信号GPIO口
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void TIM4_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	//PCLK1经过2倍频后作为TIM3的时钟源等于72MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;   //PA6通道1、PA7通道2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


}
/*
 * 函数名：TIM3_Mode_Config
 * 描述  ：配置TIM3输出的PWM信号的模式，如周期、极性、占空比
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void TIM4_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
  u16 CCR1_Val = 1000;        //A6
  u16 CCR2_Val = 800;				//A7
  u16 CCR3_Val = 800;         //B0
  u16 CCR4_Val = 8000;        //B1
	

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 20000-1;       //当定时器从0计数到999，即为1000次，为一个定时周期  f=1M/20000=50Hz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频，即72M
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	    //设置预分频，72分频，1M
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	 //输出使能
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);	 //使能通道1
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* PWM1 Mode configuration: Channel3 */


  TIM_ARRPreloadConfig(TIM4, ENABLE);			 // 使能TIM3重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM4, ENABLE);                   //使能定时器3	
	TIM_ITConfig(TIM4, TIM_IT_CC2 , ENABLE);
}


