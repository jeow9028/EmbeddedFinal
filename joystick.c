#include "adc.h"
#include "msp.h"
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <adc.h>
#include <timer.h>

volatile uint16_t speed;
volatile uint16_t timefreq;
/*
 * Bitmapping is in overview for ADC14
 */
void joystick_configure(void){
    P4->SEL0 |= BIT4;
    P4->SEL1 |= BIT4;
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;
}

int8_t joysticklocation(uint8_t channelx, uint8_t channely){
    /*
     * Default position reads for x=8314 and for y=8147
     * Channelx takes an input from main that compares values for the left and right movement
     * Channely takes an input from main that compares values for the up and down movement
     */
    int8_t location = 0;
    if ((ADC_getN(channelx) < 12000) && (ADC_getN(channely) > 12000) && (ADC_getN(channelx) > 6000)){
        //This for for straight up
        location = 3;
        return location;
    }
    else if((ADC_getN(channelx) > 9000) && (ADC_getN(channely) > 9000)){
        //This is for quadrant 1
        location = 1;
        return location;
    }
    else if((ADC_getN(channelx) < 8000) && (ADC_getN(channely) > 9000)){
        //This is for quadrant 2
        location = 2;
        return location;
    }
    else{
        return 0;
    }
}

void pwm(uint8_t inputvalue){
    if(inputvalue == 3){
        TIMER_A1->CCTL[0] |= TIMER_A_CCTLN_CCIE;
        TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;
    }
    else if (inputvalue == 1){
        // Left
        //interrupts
        TIMER_A1->CCTL[0] |= TIMER_A_CCTLN_CCIE;
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;
    }
    else if (inputvalue == 2){
        //turn right
        // interrupts
        TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;
        TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;
    }
    else{
        TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;
    }
}

/*
 * This function changes the freqency of pwm values so it can alter speeds
 * Speed of Joystick from the input value
 * Speed 1
 *      timer 100
 * Speed 2
 *      timer 100/2
 * Speed 3
 *      timer 100 /4
 */
void Testfrequency(uint16_t speed){
    if (speed == 100){
        //output the timer flag for this

        TIMER_A1->CCTL[0] |= TIMER_A_CCTLN_CCIE;
        TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;
        timefreq = speed;
        //sprintf ("Speed %d "speed)

    }
    if (speed == 50){
        //Output the timer for speed
        TIMER_A1->CCTL[0] |= TIMER_A_CCTLN_CCIE;
        TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;
        //sprintf ("Speed %d "speed)
        timefreq = speed/2;

    }
    if (speed == 25){
        //Output the timer for speed
        TIMER_A1->CCTL[0] |= TIMER_A_CCTLN_CCIE;
        TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;
        //sprintf ("Speed %d "speed)
        timefreq = speed/4;
    }

}
