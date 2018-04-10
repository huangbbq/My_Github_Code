#include "control.h"
#include "pid.h"
#include "encoder.h"
#include "usart3.h"
#include "usart.h"

extern float angle[3],speed[3],accel[3],temperature;//角度,角速度，角加速度,温度


//角度pid的计算//
int Calc_angle_pid()
{
	float Kp=285,Ki=0,Kd=0;
	float err,Ek_last,SEK,Ek;
	
	Read_angle();
	
  //printf("%.2f   %.2f   %.2f\r\n",angle[0],angle[1],angle[2]);
  pid.Angle_Pv=angle[0];
	
	Ek=pid.Angle_Sv-pid.Angle_Pv;
	
	SEK+=Ek;
	
	err=Ek-Ek_last;
	
	pid.Angle_out=Kp*Ek+Ki*SEK+Kd*err;
	
  Ek_last=Ek;
	
  return pid.Angle_out;

}


//编码器1的pid计算//
int Calc_encoder1_pid()
{
	float Kp=0 ,Ki=0,Kd=0;
	float err,Ek_last,Ek;	
	
	pid.Encoder_Pv=Read_Encoder(2)+Read_Encoder(4);
	
	Ek=pid.Encoder_Sv-pid.Encoder_Pv;
	
  pid.Sek+=Ek;
	
  err=Ek-Ek_last;
	
  pid.Encoder_out1=Kp*Ek+Ki*pid.Sek+Kd*err;
	
	Ek_last=Ek;
	printf("左电机：%f      右电机：%f\r\n",pid.Encoder_out1,pid.right_out);
	return pid.Encoder_out1;
}


//编码器2的pid计算//
//int Calc_encoder2_pid()
//{
//	float Kp=10,Ki=0,Kd=0;
//	float err,Ek_last,SEK,Ek;	
//	
//  pid.Encoder_Pv2=Read_Encoder(4);
//	
//	Ek=pid.Encoder_Sv-pid.Encoder_Pv2;
//	
//  SEK+=Ek;
//	
//  err=Ek-Ek_last;
//	
//  pid.Encoder_out2=Kp*Ek+Ki*SEK+Kd*err;
//	
//	Ek_last=Ek;
//	
//	return pid.Encoder_out2;
//}




