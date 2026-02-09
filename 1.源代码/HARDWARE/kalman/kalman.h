#ifndef __KALMAN_H
#define __KALMAN_H	 
#include "sys.h"

typedef struct 
{	
	u32  leng1;
	u32  leng;
	u32  lengnew;
	u32  lengold;
	int leng_fix;
}length;
extern length ks;

void ks103_data(u32 data);
int kalmanfilter(int S);
int Pos_Value(u16 date);
#endif
