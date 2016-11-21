#ifndef __BSP_DWT_H
#define __BSP_DWT_H

void BSP_InitDWT (void);
void BSP_DelayUS (uint32_t _ulDelayTime);
void BSP_DelayMS (uint32_t _ulDelayTime);
uint32_t BSP_GetTick (void);
#endif
