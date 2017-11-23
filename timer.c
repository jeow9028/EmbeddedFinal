#include <gpio1.h>
#include "msp.h"
#include "timer.h"


volatile uint16_t tCount; //keep track of timer interrupts
volatile uint16_t counter;
/*
void timerA1_config(){
    TIMER_A1->R = 0;                          // Reset count
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 |
                    TIMER_A_CTL_ID_3 |
                    TIMER_A_CTL_IE |
                    TIMER_A_CTL_MC__UP| // SMCLK, enable CTL interrupts
                    TIMER_A_CTL_IFG;
    TIMER_A1->CCR[1] = 128;
    TIMER_A0->CCR[0] = 512-1;
    //TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE; //Enable CCTL interrupts
    // Enable Interrupts in the NVIC
    NVIC_EnableIRQ(TA1_0_IRQn);
}
*/
void timerA0_config(){
    TIMER_A0->R = 0;                          // Reset count
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 |
                    TIMER_A_CTL_ID_3 |
                    TIMER_A_CTL_IE |
                    TIMER_A_CTL_MC__UP| // SMCLK, enable CTL interrupts
                    TIMER_A_CTL_IFG;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
    //TIMER_A0->CCR[1] = 450;
    TIMER_A0->CCR[0] = 512-1;                 //Frequency
    //TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; //Enable CCTL interrupts
    // Enable Interrupts in the NVIC
    //NVIC_EnableIRQ(TA0_0_IRQn);
    P2->DIR |= BIT4;                       // P1.2 and P1.3 output
    P2->SEL0 |= BIT4;                       // P1.2 and P1.3 options select
    P2->SEL1 &= ~BIT4;                       // P1.2 and P1.3 options select
}
/*
void TA1_0_IRQHandler(){
    if((TIMER_A1->CCTL[0] & TIMER_A_CCTLN_CCIFG) == TIMER_A_CCTLN_CCIFG){
        if (P1->OUT & BIT5){
            P1->OUT &= ~BIT5;
        }
        else{
            //for(i=0;i<200;i++){
                P1->OUT |= BIT5;


        }
        TIMER_A1->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG); //reset flag
    }
}
*/
void TA0_0_IRQHandler(){
    /*
    if((TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG) == TIMER_A_CCTLN_CCIFG){
        if (P1->OUT & BIT6){
            P1->OUT &= ~BIT6;
        }
        else{
            //for(i=0;i<200;i++){
                P1->OUT |= BIT6;

        }
        TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG); //reset flag
    }*/
}

void pwm(uint8_t inputvalue){
    if(inputvalue == 3){
        TIMER_A0->CCR[1] = 450;

    }
    else if (inputvalue == 1){
        TIMER_A0->CCR[1] = 450;

    }
    else if (inputvalue == 2){
        TIMER_A0->CCR[1] = 213;

    }
    else{
        TIMER_A0->CCR[1] = 0;
    }
}
