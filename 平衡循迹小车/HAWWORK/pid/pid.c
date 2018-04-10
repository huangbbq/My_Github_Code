#include "pid.h"
#include "usart3.h"
#include "time.h"
PID pid;

void PID_init()
{
  pid.Angle_Sv=0;
	pid.Angle_out=0;
	pid.Angle_Pv=0;
	pid.Encoder_out1=0;
	pid.Encoder_out2=0;
	pid.Encoder_Pv1=0;
	pid.Encoder_Pv2=0;
	pid.Encoder_Sv=0;
	pid.left_out=0;
	pid.right_out=0;
	pid.T=0;
	pid.Sek=0;
	
}


