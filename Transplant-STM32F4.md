# FreeRTOS Transplant

This document which describes the process of transplanting FreeRTOS to STM32F407VET6

| Date       | Author  | Description   |
| ---------- | ------- | ------------- |
| 11/09/2023 | Manfred | First release |

**Environment**

- OS : Windows10
- Board : STM32F407VET6
- IDE : Keil5

---

If you find or have any questions, fell free to contact me.

Email : 1259106665@qq.com / manfredxxc@gmail.com

Ref:

- [STM32F4基础工程移植FreeRTOS_图图Tuu的博客-CSDN博客](https://blog.csdn.net/q361750389/article/details/106755214)
- [FreeRTOS移植STM32超详细（以STM32F103ZE为例）_freertos移植到stm32-CSDN博客](https://blog.csdn.net/dnct55/article/details/129658030)
- [FreeRTOS在STM32F4上移植_Zach_z的博客-CSDN博客](https://blog.csdn.net/zach_z/article/details/77435898)

---

**Contents**

[TOC]

## Preparation

- [x] A pre-configured demo application that should build with no errors or warnings

  ![image-20230911191311255](C:\Users\Manfred\AppData\Roaming\Typora\typora-user-images\image-20230911191311255.png)

- [x] The source code of FreeRTOS

![image-20230911191348086](C:\Users\Manfred\AppData\Roaming\Typora\typora-user-images\image-20230911191348086.png)



## Migrate files

**1.Create a folder named `FreeRTOS` in the demo project directory**

![image-20230911192146150](C:\Users\Manfred\AppData\Roaming\Typora\typora-user-images\image-20230911192146150.png)

**2.Copy all the files in `FreeRTOSv2022.12.01/FreeRTOS/Source`  to current directory `FreeRTOS`**

![image-20230911192600044](C:\Users\Manfred\AppData\Roaming\Typora\typora-user-images\image-20230911192600044.png)

**3.Copy configurations related to STM32F4 which is located at `FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_M4F_STM32F407ZG-SK` to current `include` directory**

![image-20230911194416273](C:\Users\Manfred\AppData\Roaming\Typora\typora-user-images\image-20230911194416273.png)

**4.Remove the files in folder `FreeRTOS/protable`, and keep only `Keil`/`MemMang`/`RVDS`**

![image-20230911194832511](C:\Users\Manfred\AppData\Roaming\Typora\typora-user-images\image-20230911194832511.png)



## Update demo project

**1.Create a new group named FreeRTOS_core, add the required source files.**

![image-20230911200047825](C:\Users\Manfred\AppData\Roaming\Typora\typora-user-images\image-20230911200047825.png)

The following figure shows more details

![image-20230911200412918](C:\Users\Manfred\AppData\Roaming\Typora\typora-user-images\image-20230911200412918.png)

**2.Create a new group named FreeRTOS_portable, which contains the memory management method and compiling rule**

![image-20230911200846898](C:\Users\Manfred\AppData\Roaming\Typora\typora-user-images\image-20230911200846898.png)

**3.Add the include files, three paths**

![image-20230911201827948](C:\Users\Manfred\AppData\Roaming\Typora\typora-user-images\image-20230911201827948.png)

**4.Rebuild project**

![image-20230911202212923](C:\Users\Manfred\AppData\Roaming\Typora\typora-user-images\image-20230911202212923.png)

**As the figure shows, a error occurs.** 

Open file `FreeRTOSConfig.h`, make the following modifications

```c
#ifdef __ICCARM__  //delete
#if defined(__ICCARM__) || defined(__CC_ARM)  //replace
```

**Then rebuild project again, multiply defined occurs.**

![image-20230911205116810](C:\Users\Manfred\AppData\Roaming\Typora\typora-user-images\image-20230911205116810.png)

Open the file `stm32f4xx_it.c`, comment out the functions that are defined repeatedly. Open file `FreeRTOSConfig.h` meanwhile, disable the hook function following:

```c
#define configUSE_IDLE_HOOK				0
#define configUSE_TICK_HOOK				0
#define configCHECK_FOR_STACK_OVERFLOW	0
#define configUSE_MALLOC_FAILED_HOOK	0
```

This demo project should be rebuild without any errors or warnings after finishing this



## Addition

**In most cases, basic function such as USART and time_delay is needy.**

Therefore some modification should be taken in file `sys.h` to support OS.

```c
#define SYSTEM_SUPPORT_UCOS		1	
```

Then here comes two subitem:

- `usart.c`
- `delay.c`

---

-1- `usart.c`

```c
#if SYSTEM_SUPPORT_UCOS
#include "FreeRTOS.h"
#endif

/* There’s no need to use `OSIntEnter/OSIntExit` when using FreeRTOS */
void USART1_IRQHandler(void)
{

//#ifdef OS_TICKS_PER_SE
//	OSIntEnter();    
//#endif
    ...
//#ifdef OS_TICKS_PER_SEC
//	OSIntExit();  											 
//#endif

}
```

-2- `delay.c`

```c
#if SYSTEM_SUPPORT_UCOS
#include "FreeRTOS.h"
#include "task.h"
#endif

extern void xPortSysTickHandler(void); 
void SysTick_Handler(void)
{				   
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//??????
    {
        xPortSysTickHandler();  
    }
}

void delay_us(u32 nus)
{		
	...
//	OSSchedLock();
	...
//	OSSchedUnlock();							    
}

void delay_ms(u16 nms)
{	
		if(OSRunning==OS_TRUE&&OSLockNesting==0)
	{		  
		if(nms>=fac_ms)
		{
   			vTaskDelay(nms/fac_ms); //use vTaskDelay() instead of OSTimeDly()
            // vTaskDelay() places the calling task into the Blocked state
		}
		nms%=fac_ms;   
	}
	delay_us((u32)(nms*1000));
}
```























