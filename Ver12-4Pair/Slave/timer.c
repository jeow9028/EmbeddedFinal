/******************************************************************************
 * @Title: timer.c
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

#include <gpio1.h>
#include "msp.h"
#include "timer.h"
volatile uint16_t tCount; //keep track of timer interrupts
volatile uint16_t valueb;
volatile uint16_t lastvalue = 0xA;
volatile uint16_t i = 0;
volatile uint16_t value;

//#define TIMERINTERRUPTS

/*
 * Function: timerA0_config
 * ----------------------------
 *   First Speed Control PWM Configuration
 *
 *   !Note: TIMER_A0->CCTL[x] (from 0-6) MUST be set in order for PWM to work.
 */
void timerA0_config(){
    TIMER_A0->R = 0;                           // Reset count
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 |     // SMCLK
                    TIMER_A_CTL_ID_3 |         // Divider = 8
                    //TIMER_A_CTL_IE |           // Timer A0 Interrupt Enable
                    TIMER_A_CTL_MC__UP|        // Up Mode
                    TIMER_A_CTL_IFG;           // Timer A0 Interrupt Flag
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;// Reset/Set PWM for A0.x where 0<=x<=6
    TIMER_A0->CCR[0] = 200;                    // Frequency
#ifdef TIMERINTERRUPTS
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;  // Enable CCTL interrupts
    NVIC_EnableIRQ(TA0_0_IRQn);
#endif
}

/*
 * Function: timerA1_config
 * ----------------------------
 *   Second Speed Control PWM Configuration
 *
 *   Note that CCTL has 6 Capture/Compare Modes.
 *   We are using Compare Mode for PWM. Change and/or
 *   configure TIMER_A1->CCTL[x] accordingly.
 */
void timerA1_config(){
    TIMER_A1->R = 0;                           // Reset count
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 |     // SMCLK
                    TIMER_A_CTL_ID_3 |         // Divider = 8
                    //TIMER_A_CTL_IE |           // Timer A0 Interrupt Enable
                    TIMER_A_CTL_MC__UP|        // Up Mode
                    TIMER_A_CTL_IFG;           // Timer A0 Interrupt Flag
    TIMER_A1->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;// Reset/Set
    TIMER_A1->CCR[0] = 200;                    // Frequency
}
/*
 * Function: pwm
 * ----------------------------
 *   Main Wave Generator. TIMER_A0->CCR[x] allows for different
 *   duty cycles to be generated that varies the output of the
 *   speed controller.
 *   TIMER_A0->CCR[1] uses P2.4, while CCR[2] uses P2.5. This information
 *   is found in the gpio1.c file.
 *
 *   Using enumerations for the duty cycle readings. It makes the code cleaner.
 *   The enumeration function is found in timer header file.
 *
 *   NOTE! p7.7 is TA1.1. This will be used for the RIGHT Motor
 *         p2.5 is TA0.2. This will be used for the LEFT Motor
 */
void pwm(uint8_t inputvalue){
//FORWARD
    if(inputvalue == 4){
        P1->OUT &= ~BIT7;
        P1->OUT &= ~BIT6;
        TIMER_A0->CCR[2] = max;
        TIMER_A1->CCR[1] = max;
    }
    else if (inputvalue == 3){
        P1->OUT &= ~BIT7;
        P1->OUT &= ~BIT6;
        TIMER_A0->CCR[2] = twentyfive;
        TIMER_A1->CCR[1] = twentyfive;
    }
//RIGHT
    else if (inputvalue == 2){
        P1->OUT &= ~BIT7;
        P1->OUT &= ~BIT6;
        TIMER_A0->CCR[2] = max;
        TIMER_A1->CCR[1] = fifty;
    }
    else if (inputvalue == 1){
        P1->OUT &= ~BIT7;
        P1->OUT &= ~BIT6;
        TIMER_A0->CCR[2] = fifty;
        TIMER_A1->CCR[1] = twentyfive;
    }
    else if (inputvalue == 8){
        P1->OUT |= BIT7;
        TIMER_A0->CCR[2] = twentyfive;
        TIMER_A1->CCR[1] = twentyfive;
    }
//LEFT
    else if (inputvalue == 5){
        P1->OUT &= ~BIT7;
        P1->OUT &= ~BIT6;
        TIMER_A0->CCR[2] = twentyfive;
        TIMER_A1->CCR[1] = fifty;
    }
    else if (inputvalue == 6){
        P1->OUT &= ~BIT7;
        P1->OUT &= ~BIT6;
        TIMER_A0->CCR[2] = fifty;
        TIMER_A1->CCR[1] = max;
    }
    else if (inputvalue == 7){
        P1->OUT |= BIT6;
        TIMER_A0->CCR[2] = twentyfive;
        TIMER_A1->CCR[1] = twentyfive;
    }
    else if (inputvalue == 9){
        P1->OUT |= BIT6;
        P1->OUT |= BIT7;
        TIMER_A0->CCR[2] = fifty;
        TIMER_A1->CCR[1] = fifty;
    }
//NOTHING
    else{
        P1->OUT &= ~BIT7;
        P1->OUT &= ~BIT6;
        TIMER_A0->CCR[2] = zero;
        TIMER_A1->CCR[1] = zero;
    }
}

uint16_t retriever(void){
    if(i<1000){
        valueb = EUSCI_A2->RXBUF;
        if(valueb < 10){
            value = valueb;
        }
        else if(valueb == 0xA && lastvalue != valueb){
            i=0;
        }
        lastvalue = valueb;
        i++;
    }
    else{
        value = 0;
        i = 0;
    }
    return value;
}
