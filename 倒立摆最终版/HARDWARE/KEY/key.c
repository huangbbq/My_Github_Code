#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "control.h"
#include "led.h"


u8 mode_flag1,mode_flag2,mode_flag3;

//								    
////按键初始化函数
//void KEY_Init(void) //IO初始化
//{ 
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟


//	//初始化 WK_UP-->GPIOA.0	  下拉输入
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

//}

//u8 KEY_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//按键按松开标志
//	if(mode)key_up=1;  //支持连按		  
//	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
//	{
//		delay_ms(10);//去抖动 
//		key_up=0;
//		if(KEY0==0)return KEY0_PRES;
//		else if(KEY1==0)return KEY1_PRES;
//		else if(WK_UP==1)return WKUP_PRES;
//	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	    
// 	return 0;// 无按键按下
//}

//void key_Init()
//{
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
//	
//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//GPIO_Init(GPIOE, &GPIO_InitStructure);
//	
//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
//GPIO_Init(GPIOE, &GPIO_InitStructure);
//GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
//GPIO_ResetBits(GPIOE, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
//}




//int key_scan(void)
//{
//u8 KeyValue=0;
//if((GPIO_ReadInputData(GPIOE)&0xff)!=0x0f)
//{
//delay_ms(10);
//if((GPIO_ReadInputData(GPIOE)&0xff)!=0x0f)
//{
//GPIO_SetBits(GPIOE, GPIO_Pin_0);
//GPIO_ResetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
//switch(GPIO_ReadInputData(GPIOE)&0xff)
//{
//case 0x11: KeyValue = 1; break;
//case 0x21: KeyValue = 2; break;
//case 0x41: KeyValue = 3; break;
//case 0x81: KeyValue = 4;break;
//}
//GPIO_SetBits(GPIOE, GPIO_Pin_1);
//GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3);
//switch(GPIO_ReadInputData(GPIOE)&0xff)
//{
//case 0x12: KeyValue = 5; break;
//case 0x22: KeyValue = 6; break;
//case 0x42: KeyValue = 7;break;
//case 0x82: KeyValue = 8;break;
//}
//GPIO_SetBits(GPIOE, GPIO_Pin_2);
//GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3);
//switch(GPIO_ReadInputData(GPIOE)&0xff)
//{
//case 0x14: KeyValue = 1; break;
//case 0x24: KeyValue = 2; break;
//case 0x44: KeyValue = 3;break;
//case 0x84: KeyValue = 4;break;
//}
//GPIO_SetBits(GPIOE, GPIO_Pin_3);
//GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
//switch(GPIO_ReadInputData(GPIOE)&0xff){
//case 0x18: KeyValue = 5; break;
//case 0x28: KeyValue = 6; break;
//case 0x48: KeyValue = 7;break;
//case 0x88: KeyValue = 8;break;
//}
//GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
//GPIO_ResetBits(GPIOE, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
////while((GPIO_ReadInputData(GPIOE)&0x00ff)!=0x000f);
//return KeyValue;
//}
//}

//return 0;
//}

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
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
}




//GPIO端口选中方式方式选择
void EXTI15_10_IRQHandler(void){
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

void Function()
{
  u8 a;
	if(key_pressed_flag)
	{
	a=Key_scan();
	switch(a)
	{
	  case 0x01:pid.Kp=pid.Kp+1;LED0=!LED0;break;
		case 0x02:pid.Kp-=1;LED0=!LED0;break;
	  case 0x03:pid.Ti+=1000000;LED0=!LED0;break;
		case 0x04:pid.Ti-=1000000;break;
		case 0x05:pid.T+=100;break;
		case 0x06:pid.T-=100;break;
		case 0x07:pid.Td+=1;break;
		case 0x08:pid.Td-=1;break;
		case 0x0d:mode_flag1=1;mode_flag2=0;mode_flag3=0;break;
		case 0x0e:mode_flag2=1;mode_flag1=0;mode_flag3=0;break;
		case 0x0f:mode_flag3=1;mode_flag1=0;mode_flag2=0;break;
		
		

	}
  }
}




