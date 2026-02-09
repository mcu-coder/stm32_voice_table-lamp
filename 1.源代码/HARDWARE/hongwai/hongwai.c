#include "hongwai.h"  
#include "stm32f10x.h" 
#include "stm32f10x_rcc.h"  
#include "delay.h"       
                      
 void hongwai(void)  
{                   
                     
	GPIO_InitTypeDef GPIO_InitStructure;   //定义GPIO_InitTypeDef结构体
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //开启引脚时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //定义IN引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置输出功率
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化GPIOA的引脚参数,写进
	GPIO_ResetBits(GPIOB,GPIO_Pin_12); //所有引脚拉低

}



