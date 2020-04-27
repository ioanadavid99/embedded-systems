/**
  ******************************************************************************
  * @file    main.c
  * @author  Auto-generated by STM32CubeIDE
  * @version V1.0
  * @brief   Default main function.
  ******************************************************************************
*/

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include <stdio.h>

extern void initialise_monitor_handles(void);

int main(void)
{
	initialise_monitor_handles();
	printf("Hello World!\n");	// don't miss the \n when semi-hosting - use openOCD to get print statements
	for(;;);
}
