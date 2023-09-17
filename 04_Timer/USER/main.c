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
#include "timers.h"

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

	TimerHandle_t one_shot_timer;
	int one_shot_period;
	TimerHandle_t reload_timer;
	int reload_period;

	void *private_data;
}egoist, *pegoist;
pegoist chip = NULL;

typedef enum {
	SOURCE_USER,
	SOURCE_AUTO_RELOAD,
	SOURCE_ONE_SHOT,
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
	tmp->source = SOURCE_USER;
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
		status = xQueueReceive(chip->xqueue, tmp_, 0);
		if (status == pdPASS) {
			pr_info("Receive data:%s", tmp_->str);
		}
		delay_ms(1000);
	}
	// vTaskDelete(NULL);
}

static void timer_handler(TimerHandle_t timer)
{
	TickType_t time_now;
	uint32_t execute_count;

	execute_count = (uint32_t)pvTimerGetTimerID(timer);
	vTimerSetTimerID(timer, (void *)(++execute_count));

	time_now = xTaskGetTickCount();

	if (timer == chip->one_shot_timer) {
		pr_info("one_shot timer callback executing time_now[%d]\r\n", time_now);
	} else {
		pr_info("reload timer callback executing time_now[%d]\r\n", time_now);

		if (5 == execute_count) {
			xTimerStop(timer, 0);
		}
	}
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
	delay_init(168); /* 168MHZ */
	uart_init(115200);
	LED_Init();
	int ret = 0;
	BaseType_t timer_startd;

	do {

		chip = (pegoist)pvPortMalloc(sizeof(*chip));
		if (chip == NULL) {
			pr_err("\r\nfailed to alloc mem for chip\r\n");
			ret = -1;
			break;
		}
		chip->queue_size = 10;
		chip->one_shot_period = pdMS_TO_TICKS(3000);
		chip->reload_period = pdMS_TO_TICKS(500);

		chip->xqueue = (QueueHandle_t)xQueueCreate(chip->queue_size, sizeof(data));
		if (chip->xqueue == NULL) {
			pr_err("\r\nfailed to alloc mem for xqueue\r\n");
			ret = -1;
			break;
		}

		chip->one_shot_timer = xTimerCreate("OneShot", chip->one_shot_period, pdFALSE, 0, timer_handler);
		chip->reload_timer = xTimerCreate("Reload", chip->reload_period, pdTRUE, 0, timer_handler);
		if (NULL == chip->one_shot_timer || NULL == chip->reload_timer) {
			pr_err("failed to create timer\r\n");
			ret = -1;
			break;
		}

		timer_startd = xTimerStart(chip->one_shot_timer, pdMS_TO_TICKS(3000));
		if (pdFAIL == timer_startd) {
			pr_err("failed to start one_shot timer\r\n");
			ret = -1;
			break;
		}

		timer_startd = xTimerStart(chip->reload_timer, pdMS_TO_TICKS(4000));
		if (pdFAIL == timer_startd) {
			pr_err("failed to start reload timer\r\n");
			ret = -1;
			break;
		}

		// xTaskCreate(task_init, "task_init", 128, chip, 2, &chip->task_init_handler);
		vTaskStartScheduler();

		pr_info("Everything works, awesome!\r\n");

	} while (0);

	if (ret) {
		release(chip);	
	}
}

