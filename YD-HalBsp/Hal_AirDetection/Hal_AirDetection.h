#ifndef _Hal_AIRDETECTION_H
#define _Hal_AIRDETECTION_H


#define GPIO_DO_Sensor_CLK    RCC_APB2Periph_GPIOB
#define GPIO_DO_Sensor_PORT   GPIOB
#define GPIO_DO_Sensor_PIN    GPIO_Pin_6

#define GPIO_AO_Sensor_CLK    RCC_APB2Periph_GPIOA
#define GPIO_AO_Sensor_PORT   GPIOA
#define GPIO_AO_Sensor_PIN    GPIO_Pin_0


#define Read_GPIO_DO_Sensor()   GPIO_ReadInputDataBit(GPIO_DO_Sensor_PORT, GPIO_DO_Sensor_PIN)


void Hal_DO_GPIO_Init(void);
void Hal_ADC_Init(void);
void Hal_AirMQxx_Init(void);
void Hal_ReadAirMQxxData(void);



#endif


