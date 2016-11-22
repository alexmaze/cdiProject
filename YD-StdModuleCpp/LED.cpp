#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

class Led{
    public:
        Led(){
            ledInit();
        };
        void led1On();
        void led1Off();
        void led2On();
        void led2Off();
        void ledDelay();
    private:
        void ledInit(void);
        GPIO_InitTypeDef GPIO_InitStruct;	
};

void Led::ledInit(void)
{
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG, ENABLE); 
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;	
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOG, &GPIO_InitStruct);	
}

void Led::led1Off()
{
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);	
}
void Led::led2Off()
{
    GPIO_ResetBits(GPIOG, GPIO_Pin_14);
}

void Led::led1On()
{
    GPIO_SetBits(GPIOD, GPIO_Pin_13);	
}

void Led::led2On()
{
    GPIO_SetBits(GPIOG, GPIO_Pin_14);
}

void Led::ledDelay()
{
    for(int32_t i=0xfffef; i>0; i--);
}

#endif