#include "usart3.h"
#include "usart.h"
#include "string.h"
unsigned char       Temp[11],sign,counter=0;
float angle[3],speed[3],accel[3],temperature;//�Ƕ�,���ٶȣ��Ǽ��ٶ�,�¶�
unsigned char Re_buf[11],temp_buf[11];
float    t;
extern int Distance;
void usart3_init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.10  


  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  


	USART_InitStructure.USART_BaudRate =115200;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                  //ʹ�ܴ���1 

}

void USART3_IRQHandler(void)       
{	 //�����ж���Ч,���������ݼĴ�����
	 if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)  
   {
      Temp[counter] = USART_ReceiveData(USART3);   //��������

			if(counter == 0 && Temp[0] != 0x55) 
					return;      //�� 0 �����ݲ���֡ͷ������
			counter++;
			if(counter==11) //���յ� 11 ������
      { 
         memcpy(Re_buf,Temp,11);
         counter=0; //���¸�ֵ��׼����һ֡���ݵĽ���
         sign=1;
      }
   }
}


 void   Read_angle(void)
{
     //printf("%d \r\n",Distance);
  if(sign)
  {  
			 memcpy(Temp,Re_buf,11);
			 sign=0;
			 if(Re_buf[0]==0x55)       //���֡ͷ
			 {  
					switch(Re_buf[1])
					{
             case 0x51: //��ʶ������Ǽ��ٶȰ�
								accel[0] = ((short)(Temp[3]<<8 | Temp[2]))/32768.0*16;      //X����ٶ�
								accel[1] = ((short)(Temp[5]<<8 | Temp[4]))/32768.0*16;      //Y����ٶ�
								accel[2] = ((short)(Temp[7]<<8 | Temp[6]))/32768.0*16;      //Z����ٶ�
								t = ((short)(Temp[9]<<8 | Temp[8]))/340.0+36.25;      //�¶�
								break;
						 case 0x52: //��ʶ������ǽ��ٶȰ�
								speed[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*2000;      //X����ٶ�
								speed[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*2000;      //Y����ٶ�
								speed[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*2000;      //Z����ٶ�
								t  = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;      //�¶�
								break;
						 case 0x53: //��ʶ������ǽǶȰ�
								angle[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*180;   //X���ת�ǣ�x �ᣩ
								angle[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*180;   //Y�ḩ���ǣ�y �ᣩ
								angle[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*180;   //Z��ƫ���ǣ�z �ᣩ
								t = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;   //�¶�

								//printf("X��Ƕȣ�%.2f   Y��Ƕȣ�%.2f   Z��Ƕȣ�%.2f\r\n",angle[0],angle[1],angle[2]);
						    //printf("%d \r\n",Distance);
								break;
						 default:  break;
					}
			 } 	 
	 }
}

