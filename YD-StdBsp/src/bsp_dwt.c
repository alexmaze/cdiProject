/******************************************************************************
*  				User include
******************************************************************************/
#include "bsp.h"

/******************************************************************************
*  				Local Define
******************************************************************************/
/* Cortex-M4 DWT Registers addresses */
#define  DWT_CYCCNT  *(volatile unsigned int *)0xE0001004
#define  DWT_CR      *(volatile unsigned int *)0xE0001000
#define  DEM_CR      *(volatile unsigned int *)0xE000EDFC
#define  DBGMCU_CR   *(volatile unsigned int *)0xE0042004

#define  DEM_CR_TRCENA               1<<24
#define  DWT_CR_CYCCNTENA            1

/******************************************************************************
*  Function : BSP_InitDWT
* Arguments : none
*    Action : initial DWT
*   Returns : none
******************************************************************************/
void BSP_InitDWT (void)
{
	DEM_CR         |= (unsigned int)DEM_CR_TRCENA;   /* Enable Cortex-M4's DWT CYCCNT reg.  */
	DWT_CYCCNT      = (unsigned int)0u;
	DWT_CR         |= (unsigned int)DWT_CR_CYCCNTENA;
}

/******************************************************************************
*  Function : BSP_DelayMS
* Arguments : (uint32_t) _ulDelayTime, unit: ms
*    Action : great ms delay for RTOS and superloop, effective with real time
*   Returns : none
******************************************************************************/
void BSP_DelayMS (uint32_t _ulDelayTime)
{
	BSP_DelayUS(1000*_ulDelayTime);
}

/******************************************************************************
*  Function : BSP_DelayUS
* Arguments : (uint32_t) _ulDelayTime, unit: us
*    Action : great us delay for RTOS and superloop, best working on below 1s 
*   Returns : none
******************************************************************************/
void BSP_DelayUS (uint32_t _ulDelayTime)
{
    uint32_t tCnt, tDelayCnt;
	uint32_t tStart;
		
	tStart = DWT_CYCCNT;                                     /* entry count */
	tCnt = 0;
	tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);	 /* delay ticks */ 		      

	while(tCnt < tDelayCnt)
	{
		tCnt = DWT_CYCCNT - tStart;	
	}
}

/******************************************************************************
*  Function : BSP_GetTick
* Arguments : non
*    Action : get systemtick
*   Returns : none
******************************************************************************/
uint32_t BSP_GetTick (void)
{
    return DWT_CYCCNT;
}
