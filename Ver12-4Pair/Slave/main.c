/******************************************************************************
 * @Title: main.c
 *
 * @authors Brandon Lewien, Jean-Christophe Owens, Arash Yousefzadeh, Adam Smrekar
 * @date November 30th, 2017
 * @version - ---
 *
 * Compiled using CCSv7
 *
 * ***************************************************************************/
#define TIMER
#define UART
#define GPIO
#define PWM

#include "msp.h"
#include "gpio1.h"
#include "timer.h"
#include "uart.h"

volatile uint16_t value;

void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
#ifdef UART
	UART_Configure();
#endif
#ifdef TIMER
    timerA0_config();
    timerA1_config();
#endif
#ifdef GPIO
    gpio_pwmconfig();
    gpio_config();
#endif
	__enable_irq();
	while(1){
	    value = retriever();
#ifdef PWM
        pwm(value);
#endif
	}
}

