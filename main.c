#include <stdint.h>
#include "msp.h"
#include "adc.h"
#include "lcdconfig.h"
#include "joystick.h"
#include "stdio.h"

#define SCB_SCR_ENABLE_SLEEPONEXIT (0x00000002)
volatile uint16_t value;
volatile uint16_t value1;


/*
 * Function: Joystick, ADC, Timer/PWM Configuration w/out Bluetooth
 * ----------------------------
 *   function brandonjc has all the configuration function calls
 *   for joystick to pwm
 */
void brandonjc(void)
{
    lcdconfig(); // Configure LCD
    GPIO_configure(); // Configure Joystick P1 stuff
    ADC_init(); // Analog to digital configuration
    ADC_addChannel(0,15,0); //MEM0 x dir
    ADC_addChannel(4,9,0); //MEM4 y dir
    ADC_EOS(15); // enable EOS
}

/*
 * Function: Bluetooth w/ I2C
 * ----------------------------
 *   function adamarash has all the configuration function calls
 *   for bluetooth and I2C
 */
void adamarash(void)
{

}


void main(void)
{
    // Put code between lines     //*************
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;        // stop watchdog timer
    brandonjc();
    adamarash();
    __enable_irq();
    SCB->SCR &= ~SCB_SCR_ENABLE_SLEEPONEXIT;
    ADC_start();
    while(1){
        uint8_t text[128];
        //*************

        //*************

        value = joysticklocation(0,4);
        sprintf(text,"   Spd: %d   ",value);
        getwordsback(text,64,64);
    }
}
