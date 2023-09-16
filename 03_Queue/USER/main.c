#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

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

=============================================================================*/

typedef struct _egoist {
	char *name;
	TaskHandle_t task_init_handler;
	TaskHandle_t task_writer_handler;
	TaskHandle_t task_receiver_handler;

	QueueHandle_t xqueue;
	uint8_t	queue_size;

	void *private_data;
}egoist, *pegoist;
pegoist chip = NULL;

typedef enum {
	SOURCE_0,
	SOURCE_1,
	SOURCE_MAX,
}data_source;

typedef struct _data {
	u8 *str;
	data_source source;
}data;

static u16 times = 0;
void task_writer(void *param)
{
	u8 i, len;	
	BaseType_t status;
	pegoist chip = (pegoist)param;
	data *tmp = (data *)pvPortMalloc(sizeof(data));
	tmp->source = SOURCE_0;
	const TickType_t ticks_to_wait = pdMS_TO_TICKS(100);

	for ( ;; ) {
		if (USART_RX_STA & 0x8000) { /* Data have came from the serial port */
			len = USART_RX_STA & 0x3fff;
			tmp->str = USART_RX_BUF;

			status = xQueueSendToBack(chip->xqueue, tmp, ticks_to_wait);
			if (status != pdPASS) {
				pr_err("could not send to the queue\r\n");
			}

			pr_info("Input:\r\n");
			for (i = 0; i < len; i++) {
				USART_SendData(USART1, USART_RX_BUF[i]);
				while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
			}
			printf("\r\n\r\n");
			USART_RX_STA = 0;
		} else { /* nothing from the serial port */
			pr_info("Waiting data from user [t:%d]\r\n", times); 
			delay_ms(30);
		}
		times++;
	}

	// vTaskDelete(NULL);
}

void task_receiver(void *param)
{
	BaseType_t status;
	pegoist chip = (pegoist)param;
	data *tmp_ = (data *)pvPortMalloc(sizeof(data));

	for ( ;; ) {
		pr_info("It's time to receive\r\n");
		status = xQueueReceive(chip->xqueue, tmp_, 0);
		if (status == pdPASS) {
			pr_info("Receive data:%s", tmp_->str);
		}
		delay_ms(10);
	}
	// vTaskDelete(NULL);
}

void task_init(void *param)
{
	pegoist chip = (pegoist)param;

	taskENTER_CRITICAL();

	xTaskCreate(task_writer, "writer", 50, chip, 2, &chip->task_writer_handler);
	xTaskCreate(task_receiver, "receiver", 50, chip, 1, &chip->task_receiver_handler);
	vTaskDelete(chip->task_init_handler);

	taskEXIT_CRITICAL();
}

void release(pegoist chip)
{
	if (chip->xqueue != NULL) {
		vQueueDelete(chip->xqueue);
	}

	if (chip != NULL) {
		vPortFree(chip);
	}

	pr_info("Everything is gone\r\n");
}

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	LED_Init();
	int ret = 0;

	do {

		chip = (pegoist)pvPortMalloc(sizeof(*chip));
		if (chip == NULL) {
			pr_err("\r\nfailed to alloc mem for chip\r\n");
			ret = -1;
			break;
		}
		chip->queue_size = 10;

		chip->xqueue = (QueueHandle_t)xQueueCreate(chip->queue_size, sizeof(data));
		if (chip->xqueue == NULL) {
			pr_err("\r\nfailed to alloc mem for xqueue\r\n");
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

