#include <gpio1.h>
#include "msp.h"
#include "timer.h"


#define TEST2
//#define TEST1
volatile uint16_t tCount; //keep track of timer interrupts
volatile uint16_t counter;

intHandler interruptHandlers[5];

void timerA0_config(){
#ifdef TEST2
    TIMER_A0->R = 0;                          // Reset count
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 |
                    TIMER_A_CTL_ID_3 |
                    TIMER_A_CTL_IE |
                    TIMER_A_CTL_MC__UP| // SMCLK, enable CTL interrupts
                    TIMER_A_CTL_IFG;
    TIMER_A0->CCR[0] = 200;                 // Value to count to
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; //Enable CCTL interrupts
    // Enable Interrupts in the NVIC
      NVIC_EnableIRQ(TA0_0_IRQn);
#endif
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

void timerA0_enable(){
    TIMER_A0->R=0;
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP;
}
void timerA0_disable(){
    TIMER_A0->CTL &=~TIMER_A_CTL_MC__UP;
}

void TA0_0_IRQHandler()
{
    /*
    int i;
    P1->IFG |= BIT6;
    for(i=0;i<5;i++) {
        if(TIMER_A0->CCTL[i] & TIMER_A_CCTLN_CCIFG && interruptHandlers[i])
        {
            TIMER_A0->CCTL[i]&=~TIMER_A_CCTLN_CCIFG;
            interruptHandlers[i]();
        }
    }
    */
    if((TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG) == TIMER_A_CCTLN_CCIFG)
    {
        int i =0;
        tCount++;
        P1->OUT |= BIT6;


        if (tCount == 100){
            P1->OUT &= ~BIT6;
            for(i = 0;i <100000;i++);
            tCount = 0;
        }
        TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG);
    }
}


