#include "sys.h"
#include "usart2.h"
#include "To_Lx.h"
#include "HC_SR04.h"
u8 ST_Sent[50];
u8 a,b;
u16 sonic_dis;


void ST_Send(void)
{
	u8 _cnt=0;
	u8 i=0;
	u8 sum = 0;
	
	ST_Sent[_cnt++]=0xAA;
	ST_Sent[_cnt++]=0xAA;
	ST_Sent[_cnt++]=0xF1;
	ST_Sent[_cnt++]=0;
	
	ST_Sent[_cnt++]=BYTE1(sonic_dis); //发送数据1
	ST_Sent[_cnt++]=BYTE0(sonic_dis); //发送数据1
//	ST_Sent[_cnt++]=BYTE1(x2); //发送数据2
//	ST_Sent[_cnt++]=BYTE0(x2); //发送数据2
	
	ST_Sent[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += ST_Sent[i];
		
	ST_Sent[_cnt++] = sum;
	
	Uart2SendBuf(ST_Sent, _cnt);
}
void ST_Duty(u8 *data_buf)
{
	switch( *(data_buf+2) )
	{
		case 0xF1://点检测数据
				a = ((s16)(*(data_buf+4)<<8)|*(data_buf+5));
				b = ((s16)(*(data_buf+6)<<8)|*(data_buf+7));
		break;
		case 0xF2://点检测数据
			
		break;
		case 0xF3://线检测数据
		
		break;	
	}
}
void ST_Receive(u8 data)
{
	static u8 RxBuffer[32];
	static u8 _data_len = 0,_data_cnt = 0;
	static u8 state = 0;
	
	switch(state)
	{
		case 0:
			if(data==0xAA)
			{
				state=1;
				RxBuffer[0]=data;
			}else state = 0;
		break;
			
		case 1:
			if(data==0xAA)
			{
				state=2;
				RxBuffer[1]=data;
			}else state = 0;
		break;
			
		case 2:
			if(data<0xFF)
			{
				state=3;
				RxBuffer[2]=data;
			}else state = 0;
		break;
			
		case 3:
			if(data<33)
			{
				state = 4;
				RxBuffer[3]=data;
				_data_len = data;
				_data_cnt = 0;
			}else state = 0;
		break;

		case 4:
			if(_data_len>0)
			{
				_data_len--;
				RxBuffer[4+_data_cnt++]=data;
				if(_data_len==0)	state = 5;
			}else state = 0;
		break;			
			
		case 5:
			state = 0;
			RxBuffer[4+_data_cnt]=data;
			ST_Duty(RxBuffer);	
		break;	
		default:	state = 0;	break;
	}
}
