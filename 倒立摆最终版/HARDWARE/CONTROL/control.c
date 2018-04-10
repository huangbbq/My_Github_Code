#include <stdlib.h>
#include "control.h"
#include "adc.h"
#include "bianliang.h"
#include "QD.h"
#include "pwm.h"
#include "encoder.h"

PID pid;
u8 sign_flag;
int target=1387;                            //期望角度
void PID_Init()
{
	pid.Sv=3070;
	pid.Kp=12810;		      //12810
	pid.T=10.3005;	           	//10.3005
	pid.Ti=4557832.5;     //4557832.5
	pid.Td=51.3;            //51.3
	pid.OUT0=1;
	pid.SEK1=0;
	pid.SEk2=0;
	pid.OUT=0;
	pid.Pout=0;
	pid.angle_out=0;
	pid.enconter_out=0;
	pid.Pv=0;
}

//void pid_control(void)
//{
//	float DelEk;
//	float Ki,Kd;	
////if(pid.time<=pid.T/10)			//time 为10ms 一次	T为1ms一次
////	{
////		return ;
////	}
// if(pid.Pv>=2800&&pid.Pv<=3350)
// {
//	
//		pid.Ek=pid.Sv-pid.Pv;				//当前偏差值=设定值-当前值（1）  主函数中调用采样adc值
//		
//		pid.SEK+=pid.Ek;						//历史偏差总和（2）	
//		

//		DelEk=pid.Ek-pid.Ek_1; 			//最近两次偏差（3）
//		
//		Ki=pid.Kp*(pid.T/pid.Ti);							//积分项系数
//		 
//		Kd=(pid.Td/pid.T)*pid.Kp;	//微分系数
//		

//		pid.Iout=pid.SEK*pid.Kp*Ki/1000;			//积分项
//		
////		if(pid.Iout>100)		//pwm周期  主函数中设置
////		{
////			pid.Iout=100;	
////		}
////		else if(pid.Iout<-100)
////		{
////			pid.Iout=-100;
//////		}	
//				
//		pid.Pout=pid.Kp*pid.Ek/1000;					//比例项
//		
//		
//		pid.Dout=DelEk*Kd/1000;
//		
////			if(pid.Dout>900)		//pwm周期  主函数中设置
////		{
////			pid.Dout=900;	
////		}
////		else if(pid.Dout<-900)
////		{
////			pid.Dout=-900;
////		}	
//		pid.OUT=pid.Pout+pid.Iout+pid.Dout*0;				//最终计算结果
//		
//		if(pid.OUT>3000)		//pwm周期  主函数中设置3
//		{
//			pid.OUT=3000;	
//		}
//		else if(pid.OUT<-3000)
//		{
//			pid.OUT=-3000;
//		}	

//		pid.Ek_1=pid.Ek;		
//		pid.time=0;				//T周期到时，进行计算后置零；在tim2中断中设置
//		sign_flag=1;
//}
//}
int velocity_mode1()
{  
    static float Velocity,Encoder_Least,Encoder,S_encoder=200;
	  static float encoder_last,err;
	  static int i=0;
	  float kp=50.09,ki=kp/8.522,kd=62.808;  
	
   //=============速度PI控制器=======================//	
		Encoder_Least=Read_Encoder();   //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零）	
		Encoder=S_encoder-Encoder_Least;
	  err=Encoder-encoder_last;
//		Encoder *= 0.7;		                               //===一阶低通滤波器       
//		Encoder += Encoder_Least*0.3;	                   //===一阶低通滤波器    
		pid.SEk2 +=Encoder;                      //===积分出位移 积分时间：10ms
		if(pid.SEk2>7100)  	pid.SEk2=7100;  //===积分限幅
		if(pid.SEk2<-7100)	pid.SEk2=-7100;   //===积分限幅	
	
		Velocity=Encoder*kp+pid.SEk2*ki+err*kd;          //===速度控制	
	
	  encoder_last=Encoder;
	  i++;

	  return Velocity;
}

int balance_mode1()
{  
	//假设Kp值为720时，小车在±10°时就会慢转。故确定范围为0~720.从小逐渐增大Kp直到低频的抖动出现
	//观察陀螺仪的原始数据知道速度不会超过多少位数，再由7200表示满占空比，所以确定Kd值大小
	//Kp和Kd联合调试结果是出现抖动前参数越大越好。
   static float Bias,Bias_last;
	 float kp=11.35,kd=90.095,ki=0.399906,err;               //kp是P系数 kd是D系数 kp=53,kd=150;
	 int balance;
	 pid.Pv=Get_Adc_Average(ADC_Channel_1,5);        //读出角度值 
	 Bias=pid.Sv-pid.Pv;                  //===求出平衡的角度中值 和机械相关
	 pid.SEK1+=Bias;
	 err=Bias-Bias_last;                  //误差变化率
	
	 balance=kp*Bias+kd*err+ki*pid.SEK1;        //===计算平衡控制的电机PWM  PD控制  
	
	 Bias_last=Bias;
	
	
	 return balance;
}

 

int velocity_mode2()
{  
    static float Velocity,Encoder_Least,Encoder,S_encoder=200;
	  static float encoder_last,err;
	  static int i=0;
	  float kp=50.09,ki=kp/8.522,kd=62.808;  
	
   //=============速度PI控制器=======================//	
		Encoder_Least=Read_Encoder();   //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零）	
		Encoder=S_encoder-Encoder_Least;
	  err=Encoder-encoder_last;
//		Encoder *= 0.7;		                               //===一阶低通滤波器       
//		Encoder += Encoder_Least*0.3;	                   //===一阶低通滤波器    
		pid.SEk2 +=Encoder;                      //===积分出位移 积分时间：10ms
		if(pid.SEk2>7100)  	pid.SEk2=7100;  //===积分限幅
		if(pid.SEk2<-7100)	pid.SEk2=-7100;   //===积分限幅	
	
		Velocity=Encoder*kp+pid.SEk2*ki+err*kd;          //===速度控制	
	
	  encoder_last=Encoder;
	  i++;

	  return Velocity;
}

int balance_mode2()
{  
	//假设Kp值为720时，小车在±10°时就会慢转。故确定范围为0~720.从小逐渐增大Kp直到低频的抖动出现
	//观察陀螺仪的原始数据知道速度不会超过多少位数，再由7200表示满占空比，所以确定Kd值大小
	//Kp和Kd联合调试结果是出现抖动前参数越大越好。
   static float Bias,Bias_last;
	 float kp=13.35,kd=80.095,ki=0.399906,err;               //kp是P系数 kd是D系数 kp=53,kd=150;
	 int balance;
	 pid.Pv=Get_Adc_Average(ADC_Channel_1,5);        //读出角度值 
	 Bias=pid.Sv-pid.Pv;                  //===求出平衡的角度中值 和机械相关
	 pid.SEK1+=Bias;
	 err=Bias-Bias_last;                  //误差变化率
	
	 balance=kp*Bias+kd*err+ki*pid.SEK1;        //===计算平衡控制的电机PWM  PD控制  
	
	 Bias_last=Bias;
	
	
	 return balance;
}


int velocity_mode3()
{  
    static float Velocity,Encoder_Least,Encoder,S_encoder=200;
	  static float encoder_last,err;
	  static int i=0;
	  float kp=36.09,ki=kp/300.22,kd=43.808;  
	
   //=============速度PI控制器=======================//	
		Encoder_Least=Read_Encoder();   //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零）	
		Encoder=S_encoder-Encoder_Least;
	  err=Encoder-encoder_last;
//		Encoder *= 0.7;		                               //===一阶低通滤波器       
//		Encoder += Encoder_Least*0.3;	                   //===一阶低通滤波器    
		pid.SEk2 +=Encoder;                      //===积分出位移 积分时间：10ms
		if(pid.SEk2>7100)  	pid.SEk2=7100;  //===积分限幅
		if(pid.SEk2<-7100)	pid.SEk2=-7100;   //===积分限幅	
	
		Velocity=Encoder*kp+pid.SEk2*ki+err*kd;          //===速度控制	
	
	  encoder_last=Encoder;
	  i++;

	  return Velocity;
}

int balance_mode3()
{  
	//假设Kp值为720时，小车在±10°时就会慢转。故确定范围为0~720.从小逐渐增大Kp直到低频的抖动出现
	//观察陀螺仪的原始数据知道速度不会超过多少位数，再由7200表示满占空比，所以确定Kd值大小
	//Kp和Kd联合调试结果是出现抖动前参数越大越好。
   static float Bias,Bias_last;
	 float kp=13.35,kd=46.095,ki=0.279906,err;               //kp是P系数 kd是D系数 kp=53,kd=150;
	 int balance;
	 pid.Pv=Get_Adc_Average(ADC_Channel_1,5);        //读出角度值 
	 Bias=pid.Sv-pid.Pv;                  //===求出平衡的角度中值 和机械相关
	 pid.SEK1+=Bias;
	 err=Bias-Bias_last;                  //误差变化率
	
	 balance=kp*Bias+kd*err+ki*pid.SEK1;        //===计算平衡控制的电机PWM  PD控制  
	
	 Bias_last=Bias;
	
	
	 return balance;
}
