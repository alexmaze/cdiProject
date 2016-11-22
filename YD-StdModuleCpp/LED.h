#ifndef _LED_H_
#define _LED_H_


#include "stm32f1xx_hal.h"
//#include "stm32f10x.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"

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

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void Led::led1Off()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
}
void Led::led2Off()
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

void Led::led1On()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
}

void Led::led2On()
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

void Led::ledDelay()
{
//    HAL_Delay(1000);
	for(int32_t i=0xfffff; i>0; i--);
}

#endif