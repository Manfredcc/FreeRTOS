#include "delay.h"
#include "sys.h"

#if SYSTEM_SUPPORT_UCOS
#include "FreeRTOS.h"
#include "task.h"
#define OS_CRITICAL_METHOD
#endif

static u8  fac_us=0;
static u16 fac_ms=0;

#ifdef OS_CRITICAL_METHOD

extern void xPortSysTickHandler(void); 
void SysTick_Handler(void)
{				   
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED) /* System had been run */
    {
        xPortSysTickHandler();  
    }
}

#endif
			   
void delay_init(u8 SYSCLK)
{
#ifdef OS_CRITICAL_METHOD
	u32 reload;
#endif
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SYSCLK/8;
	    
#ifdef OS_CRITICAL_METHOD
	reload=SYSCLK/8;
	reload*=1000000/configTICK_RATE_HZ; /* time slice = 1,000,000ms / configTICK_RATE_HZ  */

	fac_ms=1000/configTICK_RATE_HZ;  
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;
	SysTick->LOAD=reload;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
#else
	fac_ms=(u16)fac_us*1000;
#endif
}								    

#ifdef OS_CRITICAL_METHOD
	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;  	 
	ticks=nus*fac_us;		 
	tcnt=0;
//	OSSchedLock();
	told=SysTick->VAL;
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;
		}  
	};
//	OSSchedUnlock();							    
}

void delay_ms(u16 nms)
{	
		if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
	{		  
		if(nms>=fac_ms)
		{
   			vTaskDelay(nms/fac_ms);
		}
		nms%=fac_ms; 
	}
	delay_us((u32)(nms*1000));
}

/* 168M,nms<=798ms */
void delay_xms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;
	SysTick->VAL =0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;
} 
#else

void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //??????	  		 
	SysTick->VAL=0x00;        //????????
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //??????? 
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//????????   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //????????
	SysTick->VAL =0X00;       //????????	 
}
//???nms
//???nms???¦¶
//SysTick->LOAD?24¦Ë?????,????,???????:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK??¦Ë?Hz,nms??¦Ë?ms
//??168M??????,nms<=798ms 
void delay_xms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//??????(SysTick->LOAD?24bit)
	SysTick->VAL =0x00;           //????????
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //???????  
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//????????   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //????????
	SysTick->VAL =0X00;       //????????	  	    
} 
//???nms 
//nms:0~65535
void delay_ms(u16 nms)
{	 	 
	u8 repeat=nms/540;	//??????540,???????§»????????????,
						//???¼^???248M?????,delay_xms?????????541ms??????
	u16 remain=nms%540;
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain)delay_xms(remain);
	
} 
#endif
