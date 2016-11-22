/**
********************************************************

  ******************************************************************************
  */
  
#include "bsp_gpio.h"   

 /**
  * @brief  ��ʼ��GPIO
  * @param  ��
  * @retval ��
  */
  
void LED_GPIO_Config(void)
{		
    GPIO_InitTypeDef GPIO_InitStructure;/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE ); /*����GPIOA������ʱ��*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE ); /*����GPIOD������ʱ��*/
													   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;/*ѡ��Ҫ���Ƶ�GPIO����*/	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*��������ģʽΪͨ���������*/   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /*������������Ϊ50MHz */  
    GPIO_Init( GPIOA, &GPIO_InitStructure );/*���ÿ⺯������ʼ��GPIO*/

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    GPIO_ResetBits(GPIOD, GPIO_Pin_2);
}





void GPIO_Config( void )
{		
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;


    /*����GPIOA������ʱ��*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE ); 

    /* ����WiFiģ���Ƭѡ��CH������	��λ������RST������*/
    /*ѡ��Ҫ���Ƶ�GPIOA0��CH�����ź�GPIOA1��RST������*/															   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	

    /*��������ģʽΪͨ���������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

    /*������������Ϊ50MHz */   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

    /*���ÿ⺯������ʼ��GPIOA*/
    GPIO_Init( GPIOA, &GPIO_InitStructure );	 


    /*	*/
    GPIO_ResetBits( GPIOA, GPIO_Pin_1 );// ����WiFiģ��ĸ�λ��������	
}
/*********************************************END OF FILE**********************/
