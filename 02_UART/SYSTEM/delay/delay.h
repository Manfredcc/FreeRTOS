#ifndef __DELAY_H
#define __DELAY_H 			   
#include <sys.h>	  
//////////////////////////////////////////////////////////////////////////////////  
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com	
//********************************************************************************
//�޸�˵��
//��
////////////////////////////////////////////////////////////////////////////////// 	 
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void delay_xus(u32 nus);
void delay_xms(u16 nms);

#endif





























