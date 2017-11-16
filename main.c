#include <stdint.h>
#include "msp.h"
#include "adc.h"
#include "joystick.h"

#define SCB_SCR_ENABLE_SLEEPONEXIT (0x00000002)
volatile uint16_t value;
volatile uint16_t value1;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;        // stop watchdog timer
    GPIO_configure();
    ADC_init();
    ADC_addChannel(0,15,0);
    ADC_addChannel(4,9,0);
    ADC_addChannel(7,7,0);
    ADC_EOS(15);
    __enable_irq();
    SCB->SCR &= ~SCB_SCR_ENABLE_SLEEPONEXIT;
    ADC_start();
    while(1){
        value = joysticklocation(0,4);
    }
}
