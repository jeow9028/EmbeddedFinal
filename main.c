/******************************************************************************
 * @Title: main.c
 *
 * @authors Brandon Lewien, Jean-Christophe Owens, Arash Yousefzadeh, Adam Smrekar
 * @date November 23rd, 2017
 * @version - ---
 *
 * Compiled using CCSv7
 *
 * Note that LCD will mess with other pin-out configurations. It is *highly*
 * recommended to comment-out #define LCD.
 *
 * ***************************************************************************/

#include <stdint.h>
#include "msp.h"
#include "adc.h"
#include "lcdconfig.h"
#include "joystick.h"
#include "stdio.h"
#include "timer.h"
#include "gpio1.h"

#define TIMER
//#define LCD
#define ADCJOYSTICK

#define SCB_SCR_ENABLE_SLEEPONEXIT (0x00000002)
volatile uint16_t value;
volatile uint16_t test;

/*
 * Function: Joystick, ADC, Timer/PWM Configuration w/out Bluetooth
 * ----------------------------
 *   function brandonjc has all the configuration function calls
 *   for joystick, pwm, specific gpio configurations, adc, timers,
 *   and LCD
 */
void brandonjc(void){
#ifdef ADCJOYSTICK
    joystick_configure(); // Configure Joystick P1 stuff
    ADC_init(); // Analog to digital configuration
    ADC_addChannel(0,15,0); //MEM0 x dir
    ADC_addChannel(4,9,0); //MEM4 y dir
    ADC_EOS(15); // enable EOS
    ADC_start();
#endif
#ifdef TIMER
    timerA0_config();
    //timerA1_config();
    gpio_pwmconfig();
    gpio_config();
#endif
#ifdef LCD
    lcdconfig(); // Configure LCD
#endif
}

/*
 * Function: Bluetooth w/ I2C
 * ----------------------------
 *   function adamarash has all the configuration function calls
 *   for bluetooth and I2C
 */
void adamarash(void){
}

void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    SCB->SCR &= ~SCB_SCR_ENABLE_SLEEPONEXIT;
    brandonjc();
    adamarash();
    __enable_irq();
    while(1){
#ifdef ADCJOYSTICK
        value = joysticklocation(0,4);
#endif


#ifdef TIMER
        pwm(value);
#endif
#ifdef LCD
        char text[128];
        sprintf(text,"   Quadrant: %d   ",value);
        getwordsback(text,64,64);
#endif
    }
}
