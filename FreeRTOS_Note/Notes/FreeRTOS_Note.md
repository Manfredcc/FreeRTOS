# Mastering the FreeRTOS > Note

| Date       | Author  | Description   |
| ---------- | ------- | ------------- |
| 2023/08/30 | Manfred | First release |

This document is solely owned, developed and maintained by Manfred(Me). To be honest, it’s my first time develop using FreeRTOS. I learn official document **Mastering the FreeRTOS** and practice it. I feel like Alice through the looing glass, it’s going be an adventure.

If you have any question and suggestions, feel free to contract me by following methods:

- Email : manfredxxc@foxmail.com

---

**Environment**

- **Microprocessor** : <u>STM32F407VET6</u>
- **IDE** : <u>KEIL5</u>
- **Kernel** : <u>FreeRTOSv202212.01</u>



# Preface

## **About FreeRTOS**

*FreeRTOS is solely owned, developed and maintained by **Real Time Engineers Ltd***

- It’s ideally suited to deeply embedded real-time application that use microcontrollers or small microprocessors. It supports a mix of **both hard and soft real-time requirements**.

- The kernel organized  applications as **a collection of independent threads of execution.**

- On **one core, only a single thread can be executing** at any one time.

- The **kernel decides which thread should be executing by assigned priority**.

- To **meet hard real-time requirements**, designer could assign higher priorities to threads

---

综上所述，FreeRTOS是**Real Time Engineers**有限公司开发并维护的一个实时操作系统，通过事先给`thread`分配优先级的方式，实现对软实时和硬实时的支持。FreeRTOS将应用程序组织为由众多`thread`组成的集合，同一时间，一个处理器核心只能运行一个`thread`，它们的调度和事先分配的优先级挂钩。



## In addition

- Each `thread `of execution is called a `task`.

- The kernel is responsible for execution timing and provide a time-related API to the application.

- Software is completely event-driven, so no processing time is wasted by polling for events that have not occurred.

- A idle task is created automatically when the scheduler is started. It executes whenever there are no application task wishing to execute. It could simply to place the processor into a low-power mode.

- Interrupt handlers can be kept very short by deferring processing.

---

As described, `task` comes as a basic element of execution in FreeRTOS. We could assign them different priorities based on events.













