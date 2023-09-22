#include "exti.h"
#include "sys.h"
#include "delay.h"
#include "key.h"
#include "debug.h"
#include "usart.h"
#include "task.h"

extern xSemaphoreHandle bin_sema;

void EXTI3_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(EXTI_GetITStatus(EXTI_Line3) != RESET) {

        xSemaphoreGiveFromISR(bin_sema, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

void EXTIX_Init(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	key_init();
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3); /* PE3 connect to inter-line 3 */

    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  
}


