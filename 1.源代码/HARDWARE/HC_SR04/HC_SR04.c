#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "key.h"
#include "HC_SR04.h"
#include "led.h"
u16 sonic_timer_10us=0;//超声波回波计数变量
u16 sonic_timer_values=0;//超声波回波高电平时间，单位us
u8 trig_enable=1;//触发使能标志
u16 sonic_timer_1ms=60;//再次触发超声波测距时间计数变量，单位1ms
u16 sonic_distance=0;//超声波测试距离  单位mm
u16 Sonic_Alarm_Values=200;//超声波距离报警值 单位mm
void hc_sr04_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PC端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
    GPIO_Init(GPIOA, &GPIO_InitStructure);//PC4
    GPIO_ResetBits(GPIOA,GPIO_Pin_9);						 //PC4输出低
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;                 //PC7 Echo 
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPD;  //输入下拉
    GPIO_Init(GPIOA,&GPIO_InitStructure);   //PC7
}
void sonic_Trig(void)
{
    if(((trig_enable==1)&&(sonic_timer_1ms>=60))||((trig_enable==0)&&(sonic_timer_1ms>=120)))//触发信号发出，收到回波信号后60ms再触发一次或者触发信号发出长时间未收到回波信号
    {
        sonic_timer_1ms=0;//触发信号发出开始计时
        Trig_1;
        delay_us(10);
        Trig_0;
        trig_enable=0;//触发使能标志清零
    }    
}
void baojing(void)
{
	 if(sonic_distance<Sonic_Alarm_Values)//判断是否超限
	 {
			LED0=!LED0;
	 }
	 else
	 {
			LED0=1;      
	 }
}
void Alarm_Values_Adjust(void)//报警值调整函数
{
    switch(KEY_Scan(0))
    {
        case S1:
        {
            if(Sonic_Alarm_Values<4000)
            {
                Sonic_Alarm_Values+=1;  
            }     
        }
        break;
        case S2:
        {
            if(Sonic_Alarm_Values<3990)
            {
                Sonic_Alarm_Values+=10;  
            }
        }
        break;
        case S3:
        {
            if(Sonic_Alarm_Values<3900)
            {
                Sonic_Alarm_Values+=100;  
            }
        }
        break;
        case S5:
        {
            if(Sonic_Alarm_Values>=1)
            {
                Sonic_Alarm_Values-=1; 
            }
        }
        break;
        case S6:
        {
            if(Sonic_Alarm_Values>=10)
            {
                Sonic_Alarm_Values-=10; 
            }
        }
        break;
        case S7:
        {
            if(Sonic_Alarm_Values>=100)
            {
                Sonic_Alarm_Values-=100;
            }
        }
        break;
        default:
        break;     
    }    
}


void EXTI9_5_IRQHandler(void)//回波信号外部中断
{
    static u8 flag_status=0;//外部中断类型标志
    if(EXTI_GetITStatus(EXTI_Line7) != RESET)	
    { 
        
        if((Echo==1)&&(flag_status==0))//上升沿触发
        {
            sonic_timer_10us=0;//回波高电平时间计数清零
            flag_status=1;
        }
        else
        if((Echo==0)&&(flag_status==1))//下降沿触发
        {
            sonic_timer_values=10*sonic_timer_10us;//回波高电平时间计数值
            printf("超声波回波高电平时间：%d us\r\n\r\n",sonic_timer_values);//串口显示回波高电平计数值  单位us
            sonic_distance=17*sonic_timer_10us;//17=((340m/s*1000mm*10us/(1000000us*2)))*10 扩大10倍
            sonic_distance=sonic_distance/10;//取整数
            printf("超声波测试距离      ：%d mm\r\n",sonic_distance);//串口线时超声波距离值
            flag_status=0;
            trig_enable=1;
            sonic_timer_1ms=0;  
        }  
        EXTI_ClearITPendingBit(EXTI_Line7);  //清除LINE7上的中断标志位  
    }
}
