#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"

/*===========================================================================
							A USART DEMO
	A project rebuilds without any error or warn, which is out of the box for
experiment on FreeRTOS mechanism.

	Date		Author			Description
	--------	---------    	---------------
	11/09/23	Manfred			First release[Support block_delay and usart]

=============================================================================*/

typedef struct _egoist {
	char *name;
	TaskHandle_t task_init_handler;
	TaskHandle_t task_0_handler;
	TaskHandle_t task_1_handler;

	void *private_data;
}egoist, *pegoist;
pegoist chip = NULL;

static u16 times = 0;
void task_0(void *param)
{
	u8 i, len;	

	for ( ;; ) {
		if (USART_RX_STA & 0x8000) { /* Data have came from the serial port */
			len = USART_RX_STA & 0x3fff;
			printf("\r\nInput:\r\n");

			for (i = 0; i < len; i++) {
				USART_SendData(USART1, USART_RX_BUF[i]);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
			}
			printf("\r\n\r\n");
			USART_RX_STA=0;
		} else { /* nothing from the serial port */
			times++;
			if (0 == times % 10) {
				printf("Waiting data from user [t:%d]\r\n", times); 
			}
			delay_ms(10);
		}
	}

	// vTaskDelete(NULL);
}

void task_1(void *param)
{
	for ( ;; ) {
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
		printf("-0- %d\r\n", times);
		delay_ms(800);
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		printf("-1- %d\r\n", times);
		delay_ms(800);

		if (0 == times % 30) {
			printf("Time flies %d\r\n", times);
		}
	}

	// vTaskDelete(NULL);
}

void task_init(void *param)
{
	pegoist chip = (pegoist)param;

	taskENTER_CRITICAL();

	xTaskCreate(task_0, "task_0", 50, chip, 2, &chip->task_0_handler);
	xTaskCreate(task_1, "task_1", 50, chip, 2, &chip->task_1_handler);
	vTaskDelete(chip->task_init_handler);

	taskEXIT_CRITICAL();
}

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	LED_Init();

	chip = (pegoist) pvPortMalloc(sizeof(*chip));
	if (chip == NULL) {
		printf("\r\nfailed to alloc mem for chip\r\n");
		return pdFAIL;
	}

	xTaskCreate(task_init, "task_init", 128, chip, 2, &chip->task_init_handler);
	vTaskStartScheduler();
}

