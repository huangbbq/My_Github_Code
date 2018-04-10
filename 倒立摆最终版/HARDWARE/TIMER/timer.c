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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx		
  	
}



void TIM6_Init(void)
{
    
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	 
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; /*������ռ���ȼ����Ծ������õ͵�*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
		 
  TIM_TimeBaseStructure.TIM_Period = 200- 1;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;	    //
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  TIM_ClearITPendingBit(TIM6, TIM_IT_Update); 
  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); //���Ϊ�ж�Դ
  TIM_ARRPreloadConfig(TIM6, ENABLE);			 // ʹ��TIM6���ؼĴ���ARR

  /* TIM6 enable counter */
   TIM_Cmd(TIM6, ENABLE);                   //ʹ�ܶ�ʱ��6	
}





//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)   //TIM2�ж�
{		
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	  {			
				
			if(mode_flag1==1)
					 {
						   
   						 pid.OUT=balance_mode1()-velocity_mode1(); 
						 
						 	printf("X%d\r\n",(int)pid.OUT);
	             while(RESET==USART_GetFlagStatus(USART1,USART_FLAG_TC));
			
	         	         if(pid.OUT>3000)		//pwm����  ������������3
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
			
	         	if(pid.OUT>3000)		//pwm����  ������������3
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
			
	         	if(pid.OUT>3000)		//pwm����  ������������3
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
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx�����жϱ�־ 		
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








