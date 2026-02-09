#ifndef __HC_SR04_H
#include "sys.h"
#define S1 0x11
#define S2 0x21
#define S3 0x41
#define S4 0x81
#define S5 0x12
#define S6 0x22
#define S7 0x42
#define S8 0x82
#define S9 0x14
#define S10 0x24
#define S11 0x44
#define S12 0x84
#define S13 0x18
#define S14 0x28
#define S15 0x48
#define S16 0x88
#define Echo GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)
#define Trig_1 GPIO_SetBits(GPIOA, GPIO_Pin_9)
#define Trig_0 GPIO_ResetBits(GPIOA, GPIO_Pin_9)
void hc_sr04_init(void);
void sonic_Trig(void);
void get_sonic_distance(void);
void Alarm_Values_Adjust(void);
extern u16 sonic_distance;
extern u16 Sonic_Alarm_Values;

#endif
