#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "control.h"
#include "led.h"


u8 mode_flag1,mode_flag2,mode_flag3;

//								    
////������ʼ������
//void KEY_Init(void) //IO��ʼ��
//{ 
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTEʱ��


//	//��ʼ�� WK_UP-->GPIOA.0	  ��������
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0

//}

//u8 KEY_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//�������ɿ���־
//	if(mode)key_up=1;  //֧������		  
//	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
//	{
//		delay_ms(10);//ȥ���� 
//		key_up=0;
//		if(KEY0==0)return KEY0_PRES;
//		else if(KEY1==0)return KEY1_PRES;
//		else if(WK_UP==1)return WKUP_PRES;
//	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	    
// 	return 0;// �ް�������
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

void KEY4x4_Init(void) //IO��ʼ��
{ 
 	
	//�������
 	GPIO_InitTypeDef GPIO_InitStructure;
		
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//ʹ��PORTEʱ��
	GPIO_InitStructure.GPIO_Pin = Keyboard_Line ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���ó��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Keyboard_Control_Port, &GPIO_InitStructure);//��ʼ��GPIOE7,8,9,10
	GPIO_SetBits(Keyboard_Control_Port,Keyboard_Line);
	

	//��������
	GPIO_InitStructure.GPIO_Pin  = Keyboard_Row;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(Keyboard_Control_Port, &GPIO_InitStructure);//��ʼ��GPIOE

}
 void Init_Keyboard_Interrupt(void)
{
	 NVIC_InitTypeDef NVIC_InitStructure;
   EXTI_InitTypeDef EXTI_InitStructure;
	 KEY4x4_Init();
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��(ʹ���жϺ�����ӳ��ʱҪʹ�ܸ��ù���ʱ��)
	 
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
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
}




//GPIO�˿�ѡ�з�ʽ��ʽѡ��
void EXTI15_10_IRQHandler(void){
	key_pressed_flag=1;
  Function();
	EXTI_ClearFlag(Keyboard_EXTI_Line);
}

u16  Key_scan(void){
	unsigned int Keyboard_Val =0;
	u32 tmpFlag=0;//������Ҫ���жϱ�־λ
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




