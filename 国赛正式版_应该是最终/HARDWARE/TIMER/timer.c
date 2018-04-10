#include "timer.h"
#include "led.h"
#include "usart.h"
#include "location.h"
#include "ov7670.h"
#include "lcd.h" 
#include "usart3.h"
 #include "pid.h"
 #include "stdlib.h"
extern u8 ov_sta,ov_sta1;	;	//在exit.c里 面定义
extern int m,n;//更新LCD显示
extern u8 tim; 			

u16 Pic_Buff[160][160];
extern u16 pt;
int camera_refresh(void)
{
	u8 j,i;
 	u16 color,pt,iz,jz;
	u32 zhi;
	if(ov_sta)//有帧中断更新？
	{	
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右  
		if(lcddev.id==0X1963)
			LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//将显示区域设置到屏幕中央
		else if(lcddev.id==0X5510||lcddev.id==0X5310)
			LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,240,320);//将显示区域设置到屏幕中央		
		LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK_H;
		
		iz=jz=pt=0;
		for(i=0;i<160;i++)
		{
			for(j=0;j<160;j++)
			{			
			OV7670_RCK_L;
			color=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK_H; 
			color<<=8;  
			OV7670_RCK_L;
			color|=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK_H; 
			
			//以下为二值法
			if(color>0x7fff|| j==159 || j==158) 
			{
				color=0xffff;//白
				Pic_Buff[j][i]=1;	
//				printf("(i%d**j%d) ",i,j);				
			}     
			else if(color<0x7fff&&color>0)
			{
			color=0;   //黑
			Pic_Buff[j][i]=0;
			pt++;	
			iz+=i;
			jz+=j;
//			printf("(i%d,,j%d) ",i,j);		
			}			
			LCD->LCD_RAM=color;						
		}
		} 
//		printf("(iz%d,jz%d,pt%d)",iz/pt,jz/pt,pt);	
		
 		ov_sta=0;					//清零帧中断标记	
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 
	zhi=(iz/pt)*1000+jz/pt;
	}
	 
	return zhi;
}	   



void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //自动重装载周期值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //预分频值  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx
 
	TIM_ITConfig(  TIM3,TIM_IT_Update  |  TIM_IT_Trigger,  ENABLE  );//使能更新触发TIM中断

	//中断分组设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		tim++;

//		printf("time:%dfps\r\n",tim);
		}
}




//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟使能
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5                                                                       	 //用于TIM3的CH2输出的PWM通过该LED显示
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_1; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7上拉	

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
 

}


//TIM1 CH1 PWM输出设置 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(u16 arr,u16 psc)
{		 					 
	
 	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //使能TIMx外设
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOA外设时钟使能
	
 
  //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用功能输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIO
 
	TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载周期值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置预分频值 不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH1 PWM2模式	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 低电平有效 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIMx

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1 预装载使能
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能,高级定时器必须开启这个 
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIMx
	   										  
} 

extern u16 t;
extern float Aim_X,Aim_Y;
//定时器6中断服务程序	 
void TIM6_IRQHandler(void)
{ 		

	u32 pp;
	u16 loc_x,loc_y,t;
	int bbq,ccp;
	float PWM_YY,PWM_XX;

	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET ) //检查指定的TIM中断发生与否:TIM 中断源 
	{	
		
		
		pp=camera_refresh();//更新显示
		loc_x=pp/1000;
		loc_y=pp%1000;
		
		Read_angle();
		if(loc_y==1 )
		{
		
		}
			
		else
		{	
			t++;
			if(t%12==0)
			{
			bbq=get_speedy(loc_y);
			ccp=get_speedx(loc_x);
				
			}
//		printf("%d",pp);
		  printf("(%d***%d)",loc_x,loc_y);
		//printf("\n");
		
		if(tim)
		{
			LCD_ShowxNum(10,10,tim,3,16,0X80);
			LCD_ShowxNum(10,25,pid.Sv_distance_X,3,16,0X80);
			LCD_ShowxNum(10,40,pid.Sv_distance_Y,3,16,0X80);
		}				
		
		
		PWM_XX=X_distance_pwm(loc_x)-(X_speed_pwm(ccp))+680;
		
		PWM_YY=Y_distance_pwm(loc_y)-Y_speed_pwm((bbq))+820;	
		
//    printf("(PWM_XX%f,SP%f)\n",PWM_XX,X_speed_pwm(ccp));			

		
		if(abs(PWM_YY-720)<=150)
		{
		   PWM_YY=720;
		}
//		if(abs(PWM_XX-720)<=150)
//		{
//		   PWM_XX=720;
//		}
		if(PWM_XX>1100) 	
		{
		PWM_XX=1100;
		}
		if(PWM_XX<300)
		{
			PWM_XX=300;
		}

		if(PWM_YY>1170)
		{
		PWM_YY=1170;
		}
		if(PWM_YY<300)
		{
		PWM_YY=300;
		}
		
		if(loc_x==0 && loc_y==0){PWM_XX=700;PWM_YY=710;}
		if(Aim_X==0 && Aim_Y==0){PWM_XX=700;PWM_YY=710;}
    TIM_SetCompare2(TIM3,PWM_YY);		//900
		TIM_SetCompare4(TIM3,PWM_XX);		//800
		

	}
}
			
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 	    
}
//基本定时器6中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM6_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig( TIM6,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//使能定时器6更新触发中断
 
	TIM_Cmd(TIM6, ENABLE);  //使能TIMx外设
 	
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 									 
}















