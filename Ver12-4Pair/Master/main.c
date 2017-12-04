/******************************************************************************
 * @Title: main.c
 *
 * @authors Brandon Lewien, Jean-Christophe Owens, Arash Yousefzadeh, Adam Smrekar
 * @date November 30th, 2017
 * @version - ---
 *
 * Compiled using CCSv7
 *
 * Notes: LCD slows things down tremendously...
 * ***************************************************************************/

#define ADC
#define JOYSTICK
#define UART
//#define LCD
#define TIMER

#include <stdint.h>
#include <stdio.h>
#include "lcdconfig.h"
#include "msp.h"
#include "adc.h"
#include "joystick.h"
#include "uart.h"
#include "timer.h"

#define SCB_SCR_ENABLE_SLEEPONEXIT (0x00000002)
volatile uint8_t value;

void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    SCB->SCR &= ~SCB_SCR_ENABLE_SLEEPONEXIT;
#ifdef LCD
    lcdconfig();                  // Configure LCD
#endif
#ifdef UART
    UART_Configure();             // Configure UART
#endif
#ifdef JOYSTICK
    joystick_configure();         // Configure Joystick P1 stuff
#endif
#ifdef ADC
    ADC_init();                   // Analog to digital configuration
    ADC_addChannel(0,15,0);       // MEM0 x dir
    ADC_addChannel(4,9,0);        // MEM4 y dir
    ADC_EOS(15);                  // Enable EOS
    ADC_start();                  // Enable ADC
#endif
#ifdef TIMER
    timerA0_config();
#endif
    __enable_irq();               // Enable Interrupts
    while(1){
#ifdef JOYSTICK
        value = joysticklocation(0,4);
#endif
#ifdef UART
        UART_sent_byte(value);
        //UART_sent_byte(0xA);
#endif
#ifdef LCD
        char text[128];
        sprintf(text,"   Quadrant: %d   ",value);
        getwordsback(text,64,64);
#endif
    }
}
