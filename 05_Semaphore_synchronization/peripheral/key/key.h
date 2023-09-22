#ifndef __KEY__H__
#define __KEY__H__

#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) /* PE4 */
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) /* PE3 */
#define KEY_UP 	    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) /* PA0 */

void key_init(void);

#endif /* __KEY__H__ */
