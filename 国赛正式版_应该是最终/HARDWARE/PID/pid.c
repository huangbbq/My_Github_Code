#include "pid.h"
#include "usart3.h"
#include "usart.h"

extern float angle[3],speed[3],accel[3],temperature;//角度,角速度，角加速度,温度
PID pid;

float Aim_X=0,Aim_Y=0;

void PID_Init(float angle_X,float angle_Y,float distance_X,float distance_Y)
{
	
  pid.Sv_angle_X=angle_X;
	pid.Sv_angle_Y=angle_Y;
	pid.Sv_distance_X=distance_X;
	pid.Sv_distance_Y=distance_Y;
	pid.Pv_speed_X=0;
	pid.Pv_spped_Y=0;
	

}

float X_distance_pwm(u16 locx)
{
	static float err,sek;
  float ek,distance_pwm;
	float pv1,ek_1;
	float kp=10,ki=0.04,kd=0.4;
	pv1=locx;
	ek=-Aim_X+pv1;
	sek+=ek;
	if(sek>3000)sek=3000;
	if(sek<-3000)sek=-3000;
	
	err=ek-ek_1;
	distance_pwm=ek*kp+ki*sek+kd*err;
	ek_1=ek;
//	printf("%f**%f*%f",kp,err,sek);
	
  return distance_pwm;

}


float Y_distance_pwm(u16 locy)
{
	static float err,sek;
  float ek,distance_pwm;
	float pv1,ek_1;
	float kp=10,ki=0.04,kd=0.4;
	pv1=locy;
	ek=-Aim_Y+pv1;
	sek+=ek;
	
	if(sek>3000)sek=3000;
	if(sek<-3000)sek=-3000;
	err=ek-ek_1;
	distance_pwm=ek*kp+ki*sek+kd*err;
	ek_1=ek;
//	printf("%f**%f*%f",kp,err,sek);
//	printf("%f***",distance_pwm);
  return distance_pwm;

}











float X_speed_pwm(int locy)
{
	static float err,sek;
  float ek,speed_pwm;
	float sv1=0,pv1,ek_1;
	float kp=400,ki=0,kd=15;//400
	pv1=locy;
	ek=sv1-pv1;
	sek+=ek;
	err=ek-ek_1;
	speed_pwm=ek*kp+ki*sek+kd*err;
	ek_1=ek;
//	printf("%f**%f*%f",kp,err,sek);
//	printf("%f***",distance_pwm);
//	printf("speed_pwm%f***",speed_pwm);
  return speed_pwm;
	
}


float Y_speed_pwm(int locy)
{
	static float err,sek;
  float ek,speed_pwm;
	float sv1=0,pv1,ek_1;
	float kp=400,ki=0,kd=15;//400
	pv1=locy;
	ek=sv1-pv1;
	sek+=ek;
	err=ek-ek_1;
	speed_pwm=ek*kp+ki*sek+kd*err;
	ek_1=ek;
//	printf("%f**%f*%f",kp,err,sek);
//	printf("%f***",distance_pwm);
	
//	printf("speed_pwm%f***",speed_pwm);
  return speed_pwm;

}
