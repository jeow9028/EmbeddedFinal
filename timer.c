#include <gpio1.h>
#include "msp.h"
#include "timer.h"

//#define TEST1
volatile uint16_t tCount; //keep track of timer interrupts
volatile uint16_t counter;

intHandler interruptHandlers[5];

void timerA1_config(){
    TIMER_A1->R = 0;                          // Reset count
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 |
                    TIMER_A_CTL_ID_3 |
                    TIMER_A_CTL_IE |
                    TIMER_A_CTL_MC__UP| // SMCLK, enable CTL interrupts
                    TIMER_A_CTL_IFG;
    TIMER_A1->CCR[0] = 200;                 // Value to count to
    TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE; //Enable CCTL interrupts
    // Enable Interrupts in the NVIC
    NVIC_EnableIRQ(TA1_0_IRQn);
}

void timerA0_config(){
    TIMER_A0->R = 0;                          // Reset count
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 |
                    TIMER_A_CTL_ID_3 |
                    TIMER_A_CTL_IE |
                    TIMER_A_CTL_MC__UP| // SMCLK, enable CTL interrupts
                    TIMER_A_CTL_IFG;
    TIMER_A0->CCR[0] = 400;                 // Value to count to
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; //Enable CCTL interrupts
    // Enable Interrupts in the NVIC
    NVIC_EnableIRQ(TA0_0_IRQn);
}

void TA1_0_IRQHandler(){
    if((TIMER_A1->CCTL[0] & TIMER_A_CCTLN_CCIFG) == TIMER_A_CCTLN_CCIFG){
        if (P1->OUT & BIT5){
            P1->OUT &= ~BIT5;
        }
        else{
            P1->OUT |= BIT5;
        }
        TIMER_A1->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG); //reset flag
    }
}

void TA0_0_IRQHandler(){
    if((TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG) == TIMER_A_CCTLN_CCIFG){
        if (P1->OUT & BIT6){
            P1->OUT &= ~BIT6;
        }
        else{
            P1->OUT |= BIT6;
        }
        TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG); //reset flag
    }
}


/*
 * This is the second timer for the second motor in conjunction of the 1st timer
 * Using the P1.5 as the port for Motor 2
 * 2nd PWM interrupt for the pin
 *
 */

