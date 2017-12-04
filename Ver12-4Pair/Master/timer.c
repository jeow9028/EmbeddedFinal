/******************************************************************************
 * @Title: gpio1.c
 *
 * @authors Brandon Lewien
 * @date November 23rd, 2017
 * @version 1.2--11/29/17BL
 *
 * Compiled using CCSv7
 *
 * Description:
 * This file contains functions to configure Pulse-Width Modulation Timers.
 * Two timers are being used with various pin-outs for different PWM
 * configurations. Refer to gpio1.c for specific pin outputs. Additional PWM
 * pins can be set over there.
 * ***************************************************************************/

#include "msp.h"
#include "timer.h"
volatile uint16_t tCount; //keep track of timer interrupts
volatile uint8_t value;
#include "uart.h"
#include "joystick.h"

#define TIMERINTERRUPTS

/*
 * Function: timerA0_config
 * ----------------------------
 */

void timerA0_config(){
    TIMER_A0->R = 0;                           // Reset count
    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2 |    // SMCLK
                    TIMER_A_CTL_ID_3 |         // Divider = 8
                    TIMER_A_CTL_IE |           // Timer A0 Interrupt Enable
                    TIMER_A_CTL_MC__UP;       // Up Mode
  //TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;// Reset/Set PWM for A0.x where 0<=x<=6
    TIMER_A0->CCR[0] = 200;                    // Frequency
#ifdef TIMERINTERRUPTS
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;  // Enable CCTL interrupts
    NVIC_EnableIRQ(TA0_0_IRQn);
#endif
}
/*
void timerA1_config(){
    TIMER_A1->R = 0;                           // Reset count
    TIMER_A1->CTL |= TIMER_A_CTL_TASSEL_2 |    // SMCLK
                    TIMER_A_CTL_ID_3 |         // Divider = 8
                    TIMER_A_CTL_IE |           // Timer A0 Interrupt Enable
                    TIMER_A_CTL_MC__UP;       // Up Mode
  //TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;// Reset/Set PWM for A0.x where 0<=x<=6
    TIMER_A1->CCR[0] = 65000;                    // Frequency
#ifdef TIMERINTERRUPTS
    TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE;  // Enable CCTL interrupts
    NVIC_EnableIRQ(TA1_0_IRQn);
#endif
}
*/
void TA0_0_IRQHandler(void){ //handles timer a0_0 interrupts
    if((TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG) == TIMER_A_CCTLN_CCIFG)
    {
        UART_sent_byte(0xA);
        //value = joysticklocation(0,4);
        //UART_sent_byte(value);
        tCount++;
        TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG);
    }
}
/*
void TA1_0_IRQHandler(void){ //handles timer a0_0 interrupts
    if((TIMER_A1->CCTL[0] & TIMER_A_CCTLN_CCIFG) == TIMER_A_CCTLN_CCIFG)
    {
        volatile uint8_t value;
        value = joysticklocation(0,4);
        UART_sent_byte(value);
        tCount++;
        TIMER_A1->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG);
    }
}
*/
