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
    TIMER_A1->CCR[0] = 100;                 // Value to count to
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
    TIMER_A0->CCR[0] = 200;                 // Value to count to
    TIMER_A2->CCR[2] = 100;
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; //Enable CCTL interrupts
    // Enable Interrupts in the NVIC
      NVIC_EnableIRQ(TA0_0_IRQn);
#ifdef TEST1
    TIMER_A0->R   = 0;         // Reset count
    TIMER_A0->CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__8 | TIMER_A_CTL_IE;       // SMCLK, Up mode
    NVIC_EnableIRQ(TA0_0_IRQn);
}

void timerA0_set(uint8_t i, uint32_t count, intHandler fn) {
    TIMER_A0->CCR[i]  = count;    // Value to count to
    TIMER_A0->CCTL[i] = TIMER_A_CCTLN_CCIE &~TIMER_A_CCTLN_CCIFG;   // TACCR0 interrupt enabled
    //interruptHandlers[i]=fn;
    TIMER_A0->CCTL[i] = ~TIMER_A_CCTLN_CAP; //capture
#endif
}

void TA1_0_IRQHandler(){
    {
        int i =0;
        tCount++;
        P1->OUT |= BIT5;


        if (tCount == 100){
            P1->OUT &= ~BIT5;
            for(i = 0;i <20000;i++);
            tCount = 0;
        }
        TIMER_A1->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG);
    }
}
void TA0_0_IRQHandler(){
    if((TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG) == TIMER_A_CCTLN_CCIFG){
        int i =0;
        tCount++;                   //counter
        P1->OUT |= BIT6;            //turn p1.6 to high
        if (tCount == 100){
            P1->OUT &= ~BIT6;       //Put to low
            for(i = 0;i <100;i++);
            tCount = 0;             // clear flag
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
void TA2_0_IRQHandler(){
    if ((TIMER_A2->CCTL[2] & TIMER_A_CCTLN_CCIFG) == TIMER_A_CCTLN_CCIFG){
        int j = 0;
        tCount++;
        P1->OUT |=BIT5;
        if (tCount == 200){
            P1->OUT &= ~BIT5;
            for (j=0; j<200; j++);
            tCount = 0;
        }
        TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG); //reset flag
    }
}
