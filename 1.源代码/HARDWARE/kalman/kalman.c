#include "kalman.h"	
#include "HC_SR04.h"

int Pos_Value(u16 date)
{
	if(date<0)date=-date;
	if(date>=0)date=date;
	return date;
}
length ks;
u8 one_Kland=0,i=0;
u32 lengold=250;
u32 sum;
void ks103_data(u32 data)
{     
	  i++;
	  ks.leng_fix = sonic_distance - lengold;
		     lengold = sonic_distance;
		     if(Pos_Value(ks.leng_fix)<=10)
			    ks.leng1=sonic_distance;
			 sum+=ks.leng1;
			 if(i==15)
			 {
				 ks.leng=1.0*sum/15*1;
				 sum=0;
				 i=0;
			 }	
	 ks.leng = kalmanfilter(ks.leng);		 
}

int kalmanfilter(int S)  
{
  static  float x_mid,x_last=0,p_mid,p_last=10,p_now;
	static  float Q=0.625,R=0.280; 
	static  float Kg;  
	int Output;
	
	x_mid = x_last;
	p_mid = p_last + Q;
	Kg=p_mid/(p_mid+R);
	Output=(long)x_mid + Kg*(S-x_mid);	
	p_now = (1-Kg)*p_mid;
	p_last = p_now;
	x_last = Output;
	return Output;
}