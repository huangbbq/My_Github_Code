#include <stdlib.h>
#include "control.h"
#include "adc.h"
#include "bianliang.h"
#include "QD.h"
#include "pwm.h"
#include "encoder.h"

PID pid;
u8 sign_flag;
int target=1387;                            //�����Ƕ�
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
////if(pid.time<=pid.T/10)			//time Ϊ10ms һ��	TΪ1msһ��
////	{
////		return ;
////	}
// if(pid.Pv>=2800&&pid.Pv<=3350)
// {
//	
//		pid.Ek=pid.Sv-pid.Pv;				//��ǰƫ��ֵ=�趨ֵ-��ǰֵ��1��  �������е��ò���adcֵ
//		
//		pid.SEK+=pid.Ek;						//��ʷƫ���ܺͣ�2��	
//		

//		DelEk=pid.Ek-pid.Ek_1; 			//�������ƫ�3��
//		
//		Ki=pid.Kp*(pid.T/pid.Ti);							//������ϵ��
//		 
//		Kd=(pid.Td/pid.T)*pid.Kp;	//΢��ϵ��
//		

//		pid.Iout=pid.SEK*pid.Kp*Ki/1000;			//������
//		
////		if(pid.Iout>100)		//pwm����  ������������
////		{
////			pid.Iout=100;	
////		}
////		else if(pid.Iout<-100)
////		{
////			pid.Iout=-100;
//////		}	
//				
//		pid.Pout=pid.Kp*pid.Ek/1000;					//������
//		
//		
//		pid.Dout=DelEk*Kd/1000;
//		
////			if(pid.Dout>900)		//pwm����  ������������
////		{
////			pid.Dout=900;	
////		}
////		else if(pid.Dout<-900)
////		{
////			pid.Dout=-900;
////		}	
//		pid.OUT=pid.Pout+pid.Iout+pid.Dout*0;				//���ռ�����
//		
//		if(pid.OUT>3000)		//pwm����  ������������3
//		{
//			pid.OUT=3000;	
//		}
//		else if(pid.OUT<-3000)
//		{
//			pid.OUT=-3000;
//		}	

//		pid.Ek_1=pid.Ek;		
//		pid.time=0;				//T���ڵ�ʱ�����м�������㣻��tim2�ж�������
//		sign_flag=1;
//}
//}
int velocity_mode1()
{  
    static float Velocity,Encoder_Least,Encoder,S_encoder=200;
	  static float encoder_last,err;
	  static int i=0;
	  float kp=50.09,ki=kp/8.522,kd=62.808;  
	
   //=============�ٶ�PI������=======================//	
		Encoder_Least=Read_Encoder();   //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩	
		Encoder=S_encoder-Encoder_Least;
	  err=Encoder-encoder_last;
//		Encoder *= 0.7;		                               //===һ�׵�ͨ�˲���       
//		Encoder += Encoder_Least*0.3;	                   //===һ�׵�ͨ�˲���    
		pid.SEk2 +=Encoder;                      //===���ֳ�λ�� ����ʱ�䣺10ms
		if(pid.SEk2>7100)  	pid.SEk2=7100;  //===�����޷�
		if(pid.SEk2<-7100)	pid.SEk2=-7100;   //===�����޷�	
	
		Velocity=Encoder*kp+pid.SEk2*ki+err*kd;          //===�ٶȿ���	
	
	  encoder_last=Encoder;
	  i++;

	  return Velocity;
}

int balance_mode1()
{  
	//����KpֵΪ720ʱ��С���ڡ�10��ʱ�ͻ���ת����ȷ����ΧΪ0~720.��С������Kpֱ����Ƶ�Ķ�������
	//�۲������ǵ�ԭʼ����֪���ٶȲ��ᳬ������λ��������7200��ʾ��ռ�ձȣ�����ȷ��Kdֵ��С
	//Kp��Kd���ϵ��Խ���ǳ��ֶ���ǰ����Խ��Խ�á�
   static float Bias,Bias_last;
	 float kp=11.35,kd=90.095,ki=0.399906,err;               //kp��Pϵ�� kd��Dϵ�� kp=53,kd=150;
	 int balance;
	 pid.Pv=Get_Adc_Average(ADC_Channel_1,5);        //�����Ƕ�ֵ 
	 Bias=pid.Sv-pid.Pv;                  //===���ƽ��ĽǶ���ֵ �ͻ�е���
	 pid.SEK1+=Bias;
	 err=Bias-Bias_last;                  //���仯��
	
	 balance=kp*Bias+kd*err+ki*pid.SEK1;        //===����ƽ����Ƶĵ��PWM  PD����  
	
	 Bias_last=Bias;
	
	
	 return balance;
}

 

int velocity_mode2()
{  
    static float Velocity,Encoder_Least,Encoder,S_encoder=200;
	  static float encoder_last,err;
	  static int i=0;
	  float kp=50.09,ki=kp/8.522,kd=62.808;  
	
   //=============�ٶ�PI������=======================//	
		Encoder_Least=Read_Encoder();   //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩	
		Encoder=S_encoder-Encoder_Least;
	  err=Encoder-encoder_last;
//		Encoder *= 0.7;		                               //===һ�׵�ͨ�˲���       
//		Encoder += Encoder_Least*0.3;	                   //===һ�׵�ͨ�˲���    
		pid.SEk2 +=Encoder;                      //===���ֳ�λ�� ����ʱ�䣺10ms
		if(pid.SEk2>7100)  	pid.SEk2=7100;  //===�����޷�
		if(pid.SEk2<-7100)	pid.SEk2=-7100;   //===�����޷�	
	
		Velocity=Encoder*kp+pid.SEk2*ki+err*kd;          //===�ٶȿ���	
	
	  encoder_last=Encoder;
	  i++;

	  return Velocity;
}

int balance_mode2()
{  
	//����KpֵΪ720ʱ��С���ڡ�10��ʱ�ͻ���ת����ȷ����ΧΪ0~720.��С������Kpֱ����Ƶ�Ķ�������
	//�۲������ǵ�ԭʼ����֪���ٶȲ��ᳬ������λ��������7200��ʾ��ռ�ձȣ�����ȷ��Kdֵ��С
	//Kp��Kd���ϵ��Խ���ǳ��ֶ���ǰ����Խ��Խ�á�
   static float Bias,Bias_last;
	 float kp=13.35,kd=80.095,ki=0.399906,err;               //kp��Pϵ�� kd��Dϵ�� kp=53,kd=150;
	 int balance;
	 pid.Pv=Get_Adc_Average(ADC_Channel_1,5);        //�����Ƕ�ֵ 
	 Bias=pid.Sv-pid.Pv;                  //===���ƽ��ĽǶ���ֵ �ͻ�е���
	 pid.SEK1+=Bias;
	 err=Bias-Bias_last;                  //���仯��
	
	 balance=kp*Bias+kd*err+ki*pid.SEK1;        //===����ƽ����Ƶĵ��PWM  PD����  
	
	 Bias_last=Bias;
	
	
	 return balance;
}


int velocity_mode3()
{  
    static float Velocity,Encoder_Least,Encoder,S_encoder=200;
	  static float encoder_last,err;
	  static int i=0;
	  float kp=36.09,ki=kp/300.22,kd=43.808;  
	
   //=============�ٶ�PI������=======================//	
		Encoder_Least=Read_Encoder();   //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩	
		Encoder=S_encoder-Encoder_Least;
	  err=Encoder-encoder_last;
//		Encoder *= 0.7;		                               //===һ�׵�ͨ�˲���       
//		Encoder += Encoder_Least*0.3;	                   //===һ�׵�ͨ�˲���    
		pid.SEk2 +=Encoder;                      //===���ֳ�λ�� ����ʱ�䣺10ms
		if(pid.SEk2>7100)  	pid.SEk2=7100;  //===�����޷�
		if(pid.SEk2<-7100)	pid.SEk2=-7100;   //===�����޷�	
	
		Velocity=Encoder*kp+pid.SEk2*ki+err*kd;          //===�ٶȿ���	
	
	  encoder_last=Encoder;
	  i++;

	  return Velocity;
}

int balance_mode3()
{  
	//����KpֵΪ720ʱ��С���ڡ�10��ʱ�ͻ���ת����ȷ����ΧΪ0~720.��С������Kpֱ����Ƶ�Ķ�������
	//�۲������ǵ�ԭʼ����֪���ٶȲ��ᳬ������λ��������7200��ʾ��ռ�ձȣ�����ȷ��Kdֵ��С
	//Kp��Kd���ϵ��Խ���ǳ��ֶ���ǰ����Խ��Խ�á�
   static float Bias,Bias_last;
	 float kp=13.35,kd=46.095,ki=0.279906,err;               //kp��Pϵ�� kd��Dϵ�� kp=53,kd=150;
	 int balance;
	 pid.Pv=Get_Adc_Average(ADC_Channel_1,5);        //�����Ƕ�ֵ 
	 Bias=pid.Sv-pid.Pv;                  //===���ƽ��ĽǶ���ֵ �ͻ�е���
	 pid.SEK1+=Bias;
	 err=Bias-Bias_last;                  //���仯��
	
	 balance=kp*Bias+kd*err+ki*pid.SEK1;        //===����ƽ����Ƶĵ��PWM  PD����  
	
	 Bias_last=Bias;
	
	
	 return balance;
}
