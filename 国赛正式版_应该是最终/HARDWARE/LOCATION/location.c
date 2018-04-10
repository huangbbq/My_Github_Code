#include "location.h"
#include "sys.h"
#include "usart.h"	

u16 save_x,save_y;

int get_speedx(u16 loca_x)
{	
	int t1=0;
	if(save_x!=0)
	{
		t1=loca_x-save_x;	
			
		printf("(Vx%d)\n",t1);
//		printf("(%d,%d)\n",save_x,loca_x);
			
	}
	save_x=loca_x;
return t1;
}

int get_speedy(u16 loca_y)
{	
	int t2=0;
	if(save_y!=0)
	{
		t2=loca_y-save_y;
		
		printf("(Vy%d,)\n",t2);
//		printf("(%d*%d)\n",save_y,loca_y);		
	}
	save_y=loca_y;	
return t2;
}

