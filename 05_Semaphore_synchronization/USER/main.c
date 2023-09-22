#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "key.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "exti.h"

/*===========================================================================
							A USART DEMO

GENERAL DESCRIPTION
	A project rebuilds without any error or warn, which is out of the box for
experiment on FreeRTOS mechanism.

	When		Who			    What, Where, Why
	--------	---    			--------------------------
	11/09/23	Manfred			First release[Support block_delay and usart]
	14/09/23	Manfred			Add stage-debug[debug.c, ref to linux]
	16/09/23	Manfred			Practice use queues of FreeRTOS[Multy task]
	17/09/23	Manfred			Practice use timer of FreeRTOS
	19/09/23	Manfred			Practice use binary-semaphore to synchronization

=============================================================================*/

xSemaphoreHandle bin_sema;

typedef struct _egoist {
	char *name;
	TaskHandle_t task_init_handler;
	TaskHandle_t period_task;
	TaskHandle_t deferred_task;

	void *private_data;
}egoist, *pegoist;
pegoist chip = NULL;

void period_task(void *param)
{
	// pegoist chip = (pegoist)param;

	for ( ;; ) {

		pr_info("All time runs\r\n");

		delay_ms(100);
	}

	// vTaskDelete(NULL);
}

void deferred_task(void *param)
{
	// pegoist chip = (pegoist)param;
	const TickType_t max_block_time = pdMS_TO_TICKS(500);

	for ( ;; ) {
		pr_info("Synchronization\r\n");
		if (xSemaphoreTake(bin_sema, max_block_time) == pdPASS) { /* A event defered from interrupt comes */
			pr_info("The event deferred by interrupt had been gotten\r\n");
			delay_ms(10); /* debouncing */
			if (0 == KEY1) {
				pr_info("Key 1 has been pressed\r\n");
			}
		}
	}

	// vTaskDelete(NULL);
}

void task_init(void *param)
{
	pegoist chip = (pegoist)param;

	taskENTER_CRITICAL();

	xTaskCreate(period_task, "period_task", 512, chip, 2, &chip->period_task);
	xTaskCreate(deferred_task, "deferred_task", 512, chip, 3, &chip->deferred_task);
	vTaskDelete(chip->task_init_handler);

	taskEXIT_CRITICAL();
}

void release(pegoist chip)
{
	if (chip != NULL) {
		vPortFree(chip);
	}

	pr_info("Everything is gone\r\n");
}

int main(void)
{ 
	delay_init(168); /* 168MHZ */
	uart_init(115200);
	key_init();
	EXTIX_Init();
	int ret = 0;

	do {

		chip = (pegoist)pvPortMalloc(sizeof(*chip));
		if (chip == NULL) {
			pr_err("\r\nfailed to alloc mem for chip\r\n");
			ret = -1;
			break;
		}

		bin_sema = xSemaphoreCreateBinary();
		if (bin_sema == NULL) {
			pr_err("failed to create bin_sema\r\n");
			ret = -1;
			break;
		}

		xTaskCreate(task_init, "task_init", 128, chip, 2, &chip->task_init_handler);
		vTaskStartScheduler();

		pr_info("Everything works, awesome!\r\n");

	} while (0);

	if (ret) {
		release(chip);	
	}
}

