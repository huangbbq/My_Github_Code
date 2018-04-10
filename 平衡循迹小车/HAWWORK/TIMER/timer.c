#include "timer.h"
#include <stdlib.h>
#include "pid.h"
#include "control.h"
#include "motor.h"
#include "usart.h"

extern float angle[3],speed[3],accel[3],temperature;//�Ƕ�,���ٶȣ��Ǽ��ٶ�,�¶�


void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx		
  	
}


void TIM3_IRQHandler(void)   //TIM3�ж�
{

  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{	     
		  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־
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
			 //printf("������%f      �ҵ����%f\r\n",pid.left_out,pid.right_out);
		 }

	   
	
	}

}

