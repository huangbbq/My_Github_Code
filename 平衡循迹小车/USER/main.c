#include "stm32f10x.h"
#include "usart.h"
#include "usart3.h"
#include "encoder.h"
#include "motor.h"
#include "timer.h"
#include "pid.h"

extern float angle[3],speed[3],accel[3],temperature;//�Ƕ�,���ٶȣ��Ǽ��ٶ�,�¶�

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  PID_init();//PID������ʼ��
  LED_Init();


  uart_init(115200);
	

	
	Encoder_Init_TIM2();//������1��ʼ��
	Encoder_Init_TIM4();//������2��ʼ��
	usart3_init();//����3���Ƕ�ֵ
	MiniBalance_Motor_Init();//���IO��ʼ��
	MiniBalance_PWM_Init(4999,0);//PWM��ʼ��
	TIM3_Int_Init(99,719); //1ms	

  while(1)
    {
//			 int z,x;
//       Read_angle();
//			 z=Read_Encoder(2);
//			 x=Read_Encoder(4);
//			 //printf("������%d   �ҵ����%d\r\n",z,x);
			
			
     }

}
