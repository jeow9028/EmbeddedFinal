#include "lcdconfig.h"
#include "msp.h"
#include "adc.h"
#include "joystick.h"
#include "port.h"

#define SCB_SCR_ENABLE_SLEEPONEXIT (0x00000002)
volatile uint16_t value;

void main(void)
{
    ADC_init();
    lcdconfig();
    GPIO_configure();
    ADC_addChannel(0,15,0);
    ADC_addChannel(4,9,0);
    ADC_addChannel(7,7,0);
    ADC_EOS(15);
    __enable_irq();
    SCB->SCR &= ~SCB_SCR_ENABLE_SLEEPONEXIT;
    ADC_start();
    while(1){
        value = joysticklocation(0,4);
        //fsprintf
        getwordsback("Type this sentence",64,4);
        getwordsback("Type another",64,64);
        getwordsback("Realist Around",64,122);
    }
}
