#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "pid.h"
#include "timer.h"

extern float Aim_X,Aim_Y;
u8 key_data[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
unsigned char Keyboard_Change_Flag =0;
u8  key_pressed_flag=0;
void Delay(vu32 nCount) {
 for(; nCount!=0;nCount--); 
}

void KEY4x4_Init(void) //IO初始化
{ 
 	
	//推挽输出
 	GPIO_InitTypeDef GPIO_InitStructure;
		
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//使能PORTE时钟
	GPIO_InitStructure.GPIO_Pin = Keyboard_Line ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置成推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Keyboard_Control_Port, &GPIO_InitStructure);//初始化GPIOE7,8,9,10
	GPIO_SetBits(Keyboard_Control_Port,Keyboard_Line);
	

	//下拉输入
	GPIO_InitStructure.GPIO_Pin  = Keyboard_Row;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(Keyboard_Control_Port, &GPIO_InitStructure);//初始化GPIOE

}
 void Init_Keyboard_Interrupt(void)
{
	 NVIC_InitTypeDef NVIC_InitStructure;
   EXTI_InitTypeDef EXTI_InitStructure;
	 KEY4x4_Init();
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟(使用中断和引脚映射时要使能复用功能时钟)
	 
	 EXTI_ClearITPendingBit(Keyboard_EXTI_Row1);
	 GPIO_EXTILineConfig(Keyboard_EXTI_PortSource,Keyboard_EXTI_PinSource1);
	 EXTI_ClearITPendingBit(Keyboard_EXTI_Row2);
	 GPIO_EXTILineConfig(Keyboard_EXTI_PortSource,Keyboard_EXTI_PinSource2);
	 EXTI_ClearITPendingBit(Keyboard_EXTI_Row3);
	 GPIO_EXTILineConfig(Keyboard_EXTI_PortSource,Keyboard_EXTI_PinSource3);
	 EXTI_ClearITPendingBit(Keyboard_EXTI_Row4);
	 GPIO_EXTILineConfig(Keyboard_EXTI_PortSource,Keyboard_EXTI_PinSource4);
	 
	  EXTI_InitStructure.EXTI_Line= Keyboard_EXTI_Line;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 
	 
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
}




//GPIO端口选中方式方式选择
void EXTI15_10_IRQHandler(void)
{
	key_pressed_flag=1;
  Function();
	EXTI_ClearFlag(Keyboard_EXTI_Line);
}

u16  Key_scan(void){
	unsigned int Keyboard_Val =0;
	u32 tmpFlag=0;//保存需要的中断标志位
	u32 ReadValue;
	u8 i=0;
	ReadValue=GPIO_ReadInputData(Keyboard_Control_Port);
	tmpFlag=(ReadValue&0x7800);
	
	switch(tmpFlag){
			case Keyboard_EXTI_Row1:
				GPIO_ResetBits(Keyboard_Control_Port ,Keyboard_Line); 
	      for(i=0;i<4;i++){
					GPIO_SetBits(Keyboard_Control_Port,(Keyboard_LineBase<<i));
					if(GPIO_ReadInputDataBit(Keyboard_Control_Port ,Keyboard_Row_1)){
						Delay(DELAY_COUNT);
						if(GPIO_ReadInputDataBit(Keyboard_Control_Port ,Keyboard_Row_1)){
									Keyboard_Val=key_data[i][0];
									Keyboard_Change_Flag =1;
									break;
							}
						}	
				}
				GPIO_SetBits(Keyboard_Control_Port ,Keyboard_Line);
				
				break;
				
				case Keyboard_EXTI_Row2:
				GPIO_ResetBits(Keyboard_Control_Port ,Keyboard_Line); 
	      for(i=0;i<4;i++){
					GPIO_SetBits(Keyboard_Control_Port,(Keyboard_LineBase<<i));
					if(GPIO_ReadInputDataBit(Keyboard_Control_Port ,Keyboard_Row_2)){
						Delay(DELAY_COUNT);
						if(GPIO_ReadInputDataBit(Keyboard_Control_Port ,Keyboard_Row_2)){
									Keyboard_Val=key_data[i][1];
									Keyboard_Change_Flag =1;
									break;
							}
						}	
				}
				GPIO_SetBits(Keyboard_Control_Port ,Keyboard_Line);
				break;
				
				case Keyboard_EXTI_Row3:
				GPIO_ResetBits(Keyboard_Control_Port ,Keyboard_Line); 
	      for(i=0;i<4;i++){
					GPIO_SetBits(Keyboard_Control_Port,(Keyboard_LineBase<<i));
					if(GPIO_ReadInputDataBit(Keyboard_Control_Port ,Keyboard_Row_3)){
						Delay(DELAY_COUNT);
						if(GPIO_ReadInputDataBit(Keyboard_Control_Port ,Keyboard_Row_3)){
									Keyboard_Val=key_data[i][2];
									Keyboard_Change_Flag =1;
									break;
							}
						}	
				}
				GPIO_SetBits(Keyboard_Control_Port ,Keyboard_Line);
				break;
				
				case Keyboard_EXTI_Row4:
				GPIO_ResetBits(Keyboard_Control_Port ,Keyboard_Line); 
	      for(i=0;i<4;i++){
					GPIO_SetBits(Keyboard_Control_Port,(Keyboard_LineBase<<i));
					if(GPIO_ReadInputDataBit(Keyboard_Control_Port ,Keyboard_Row_4)){
						Delay(DELAY_COUNT);
						if(GPIO_ReadInputDataBit(Keyboard_Control_Port ,Keyboard_Row_4)){
									Keyboard_Val=key_data[i][3];
									Keyboard_Change_Flag =1;
									break;
							}
						}	
				}
				GPIO_SetBits(Keyboard_Control_Port ,Keyboard_Line);
				break;
				
				default:
					break;
	}
	GPIO_ResetBits(Keyboard_Control_Port,Keyboard_Row);
	return Keyboard_Val;
}

u8 tim;
void Function()
{
  u8 a;
	if(key_pressed_flag)
	{
	a=Key_scan();
	switch(a)
	{
	  case 0x01:Aim_X=8;Aim_Y=17;	break;
		
		case 0x02: Aim_X=75;Aim_Y=12;		break;
         		
		case 0x03:Aim_X=145;Aim_Y=11;	break;	
					
		case 0x04:Aim_X=8;Aim_Y=84;	break;		

		case 0x05:Aim_X=76;Aim_Y=83;	break;	

		case 0x06:Aim_X=145;Aim_Y=80;	break;	

		case 0x07:Aim_X=12;Aim_Y=152;	break;

		case 0x08:Aim_X=78;Aim_Y=151;	break;		

		case 0x10:Aim_X=148;Aim_Y=151;	break;			
					
	}
  }
}


