#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t task_init_handler;
TaskHandle_t task_0_handler;
TaskHandle_t task_1_handler;

static struct str_info {
	char *name;
	int index;
	int val;
};

typedef struct _egoist {
	char *name;
	struct str_info *p_info;
}egoist, *pegoist;
static pegoist chip = NULL;

/* Debug */
void vPrintString(const char *String)
{
	taskENTER_CRITICAL();
	
	printf("%s", String);

	taskEXIT_CRITICAL();
}

static u16 times = 0;
void task_0(void *param)
{
	u8 t;
	u8 len;	
	// u16 times=0;  

	while(1)
	{
		if(USART_RX_STA&0x8000) /* Get data from user */
		{					   
			len=USART_RX_STA&0x3fff;
			printf("\r\nWhat u input is:\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
			}
			printf("\r\n\r\n");
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%10==0) //0.1s once
				printf("Waiting data t[%d]\r\n", times);  
			delay_ms(10);   
		}
	}
}

void task_1(void *param)
{
	do {
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
		delay_ms(800);
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		delay_ms(800);

		if (times % 30 == 0)
			printf("Time flies %d\r\n", times);
	} while (1);
}

void task_init(void *param)
{
	taskENTER_CRITICAL();

	xTaskCreate(task_0, "task_0", 50, NULL, 2, &task_0_handler);
	xTaskCreate(task_1, "task_1", 50, NULL, 2, &task_1_handler);
	vTaskDelete(task_init_handler);

	taskEXIT_CRITICAL();
}

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	LED_Init();
	xTaskCreate(task_init, "task_init", 128, NULL, 2, &task_init_handler);
	vTaskStartScheduler();
}

