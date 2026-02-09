#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
#include "usart.h"
#include "exti.h"
#include "timer.h"
#include "HC_SR04.h"
#include "kalman.h"
#include "lsens.h"
#include "adc.h"
#include "hongwai.h"
#include "BEEP.h"
#include "usart2.h"
#include "usart3.h"

extern u8 Res1, Res3;
extern int timer1;
u16 sonic_dis = 0;
int main(void)
{
    int key = 0, flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, guangmi = 0, flag5 = 0;
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(115200);
    

    TIM4_Int_Init(9999, 7199);    //计时
    TIM2_PWM_Init(899, 0);        //调节亮度

    BEEP = 0;
    OLED_ShowCHinese(35, 0, 0);
    OLED_ShowCHinese(50, 0, 1);
    OLED_ShowCHinese(65, 0, 2);
    OLED_ShowCHinese(80, 0, 3);      //标题

    OLED_ShowCHinese(0, 2, 6);
    OLED_ShowCHinese(15, 2, 7);
    OLED_ShowString(30, 2, ":", 16); //模式

    

    OLED_ShowCHinese(0, 6, 24);      //时间
    OLED_ShowCHinese(15, 6, 25);
    OLED_ShowString(30, 6, ":", 16);
    OLED_ShowString(65, 6, "s", 16);

    while (1)
    {
        usart2_init(9600);                                      //蓝牙通信

      

        guangmi = Lsens_Get_Val();                              //亮度
        OLED_ShowNum(110, 4, guangmi, 2, 16);

        key = KEY_Scan(0);
        if (key == 1)
        {
            flag1 += 1;
            if (flag1 > 3)
                flag1 = 0;
        }
        if (flag1 == 0)                              //智能模式
        {
            OLED_ShowCHinese(45, 2, 4);
            OLED_ShowCHinese(60, 2, 5);
            OLED_ShowCHinese(75, 2, 6);
            OLED_ShowCHinese(90, 2, 7);
           
            if (flag2 == 1)
            {

                if (guangmi > 20 && guangmi <= 40)
                {
                    TIM_SetCompare2(TIM2, 200);
                }
                if (guangmi > 40 && guangmi <= 60)
                {
                    TIM_SetCompare2(TIM2, 400);
                }
                if (guangmi > 60)
                {
                    TIM_SetCompare2(TIM2, 800);
                }
            }
            else if (flag2 == 0)
            {

                TIM_SetCompare2(TIM2, 899);//关灯
            }

        }
        if (flag1 == 1)
        {
            OLED_ShowCHinese(45, 2, 8);
            OLED_ShowCHinese(60, 2, 9);
            OLED_ShowCHinese(75, 2, 10);
           
           
						
            if (key == 3)
            {
                flag4 = !flag4; //计时
            }
            if (flag4 == 1)
            {
                TIM_Cmd(TIM4, ENABLE);  //使能TIMx
                OLED_ShowNum(40, 6, timer1, 3, 16);
            }
            if (flag4 == 0)
            {
                TIM_Cmd(TIM4, DISABLE);  //使能TIMx
                OLED_ShowNum(40, 6, timer1, 3, 16);
            }
        }

        if (flag1 == 2)
        {
            OLED_ShowCHinese(45, 2, 12);
            OLED_ShowCHinese(60, 2, 13);
            OLED_ShowCHinese(75, 2, 14);
            OLED_ShowCHinese(90, 2, 15); //远程模式

             

        }

        if (flag1 == 3)
        {
            OLED_ShowCHinese(45, 2, 26);
            OLED_ShowCHinese(60, 2, 27);
            OLED_ShowCHinese(75, 2, 28);
            OLED_ShowCHinese(90, 2, 29); //远程模式
            Uart3_SendCMD2(0x01, guangmi);
           

        }
        if (key == 4)
        {
            timer1 = 0;
        }

    }
}




