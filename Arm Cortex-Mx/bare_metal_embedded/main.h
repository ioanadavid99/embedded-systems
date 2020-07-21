/*
 * main.h
 *
 *  Created on: Jul. 18, 2020
 *      Author: idavi
 */

#ifndef MAIN_H_
#define MAIN_H_

// some stack memory calculations
#define	SIZE_TASK_STACK			1024U	// 1KB; unsigned
#define	SIZE_SCHED_STACK		1024U
#define SRAM_START				0x20000000U	// start of SRAM in memory map
#define	SRAM_SIZE				(128 * 1024)	// 128KB
#define	SRAM_END				SRAM_START + SRAM_SIZE	// this is where the RAM ends
#define T1_STACK_START           SRAM_END
#define T2_STACK_START           ( (SRAM_END) - (1 * SIZE_TASK_STACK) )
#define T3_STACK_START           ( (SRAM_END) - (2 * SIZE_TASK_STACK) )
#define T4_STACK_START           ( (SRAM_END) - (3 * SIZE_TASK_STACK) )
#define IDLE_STACK_START         ( (SRAM_END) - (4 * SIZE_TASK_STACK) )
#define SCHED_STACK_START        ( (SRAM_END) - (5 * SIZE_TASK_STACK) )

#define	TICK_HZ					1000U
#define	HSI_CLK					16000000U
#define	SYSTICK_TIM_CLK			HSI_CLK

#define	MAX_TASKS				5	// four user tasks + 1 idle task

#define DUMMY_XPSR  			0x01000000U

#define TASK_READY_STATE  		0x00
#define TASK_BLOCKED_STATE  	0XFF

#define INTERRUPT_DISABLE()  	do{__asm volatile ("MOV R0,#0x1"); asm volatile("MSR PRIMASK,R0"); } while(0)

#define INTERRUPT_ENABLE()  	do{__asm volatile ("MOV R0,#0x0"); asm volatile("MSR PRIMASK,R0"); } while(0)

// adding it here bc linker is throwing errors 
// NEED TO CHANGE THE PINS FOR THE NUCLEO BOARD!!
/*
#define LED_GREEN  12
#define LED_ORANGE 13
#define LED_RED    14
#define LED_BLUE   15

#define DELAY_COUNT_1MS 		 1250U
#define DELAY_COUNT_1S  		(1000U * DELAY_COUNT_1MS)
#define DELAY_COUNT_500MS  		(500U  * DELAY_COUNT_1MS)
#define DELAY_COUNT_250MS 		(250U  * DELAY_COUNT_1MS)
#define DELAY_COUNT_125MS 		(125U  * DELAY_COUNT_1MS)

void led_init_all(void);
void led_on(uint8_t led_no);
void led_off(uint8_t led_no);
void delay(uint32_t count);
*/ 

#endif /* MAIN_H_ */
