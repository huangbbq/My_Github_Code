#include "pwm2.h"
/*
 * ��������TIM3_GPIO_Config
 * ����  ������TIM3�����PWM�ź�GPIO��
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void TIM4_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	//PCLK1����2��Ƶ����ΪTIM3��ʱ��Դ����72MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;   //PA6ͨ��1��PA7ͨ��2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


}
/*
 * ��������TIM3_Mode_Config
 * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void TIM4_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
  u16 CCR1_Val = 1000;        //A6
  u16 CCR2_Val = 800;				//A7
  u16 CCR3_Val = 800;         //B0
  u16 CCR4_Val = 8000;        //B1
	

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 20000-1;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����  f=1M/20000=50Hz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ����72M
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	    //����Ԥ��Ƶ��72��Ƶ��1M
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	 //���ʹ��
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* PWM1 Mode configuration: Channel3 */


  TIM_ARRPreloadConfig(TIM4, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM4, ENABLE);                   //ʹ�ܶ�ʱ��3	
	TIM_ITConfig(TIM4, TIM_IT_CC2 , ENABLE);
}


