#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

void start_task(void *param);
TaskHandle_t StartTask_Handler;

int main(void)
{ 

	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
	
	xTaskCreate(start_task, "start_task", 128, NULL, 4, &StartTask_Handler);
	vTaskStartScheduler(); /* OS get involved */

	// printf("Ego: Everythings is OK, awesome!\n");
}

void task_0(void *param)
{
	do {
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
		delay_ms(800);
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		delay_ms(800);
	} while (1);
}

void task_1(void *param)
{
	do {
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		delay_ms(100);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
		delay_ms(800);
	} while (1);
}

void start_task(void *param)
{
	taskENTER_CRITICAL(); /* Enter the critical region */

	xTaskCreate(task_0, "task_0", 50, NULL, 4, NULL);
	xTaskCreate(task_1, "task_1", 50, NULL, 4, NULL);

	vTaskDelete(StartTask_Handler); /* delete myself */

	taskEXIT_CRITICAL(); /* Exit the critical region */
}
 



