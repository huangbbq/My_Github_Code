#include "usart3.h"
#include "usart.h"
#include "string.h"
unsigned char       Temp[11],sign,counter=0;
float angle[3],speed[3],accel[3],temperature;//角度,角速度，角加速度,温度
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.9
   
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.10  


  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  


	USART_InitStructure.USART_BaudRate =115200;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART3, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                  //使能串口1 

}

void USART3_IRQHandler(void)       
{	 //接收中断有效,若接收数据寄存器满
	 if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)  
   {
      Temp[counter] = USART_ReceiveData(USART3);   //接收数据

			if(counter == 0 && Temp[0] != 0x55) 
					return;      //第 0 号数据不是帧头，跳过
			counter++;
			if(counter==11) //接收到 11 个数据
      { 
         memcpy(Re_buf,Temp,11);
         counter=0; //重新赋值，准备下一帧数据的接收
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
			 if(Re_buf[0]==0x55)       //检查帧头
			 {  
					switch(Re_buf[1])
					{
             case 0x51: //标识这个包是加速度包
								accel[0] = ((short)(Temp[3]<<8 | Temp[2]))/32768.0*16;      //X轴加速度
								accel[1] = ((short)(Temp[5]<<8 | Temp[4]))/32768.0*16;      //Y轴加速度
								accel[2] = ((short)(Temp[7]<<8 | Temp[6]))/32768.0*16;      //Z轴加速度
								t = ((short)(Temp[9]<<8 | Temp[8]))/340.0+36.25;      //温度
								break;
						 case 0x52: //标识这个包是角速度包
								speed[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*2000;      //X轴角速度
								speed[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*2000;      //Y轴角速度
								speed[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*2000;      //Z轴角速度
								t  = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;      //温度
								break;
						 case 0x53: //标识这个包是角度包
								angle[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*180;   //X轴滚转角（x 轴）
								angle[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*180;   //Y轴俯仰角（y 轴）
								angle[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*180;   //Z轴偏航角（z 轴）
								t = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;   //温度

								//printf("X轴角度：%.2f   Y轴角度：%.2f   Z轴角度：%.2f\r\n",angle[0],angle[1],angle[2]);
						    //printf("%d \r\n",Distance);
								break;
						 default:  break;
					}
			 } 	 
	 }
}

