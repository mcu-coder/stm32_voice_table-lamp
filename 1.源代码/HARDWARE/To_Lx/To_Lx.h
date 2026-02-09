#ifndef _TO_LX_H
#define _TO_LX_H
#include "usart2.h"
#include "sys.h"
void ST_Send(void);
void ST_Duty(u8 *data_buf);
void ST_Receive(u8 data);
#endif
