#include <gpio1.h>
#include <stdlib.h>
#include "joystick.h"
#include "timer.h"

intHandler interruptHandlers[7][8]; //probably will remove

void gpio_config() {
    NVIC_EnableIRQ(PORT1_IRQn);
    //NVIC_EnableIRQ(PORT2_IRQn);
    //NVIC_EnableIRQ(PORT3_IRQn);
    //NVIC_EnableIRQ(PORT4_IRQn);
    //NVIC_EnableIRQ(PORT5_IRQn);
    NVIC_SetPriority(PORT1_IRQn,1); //not necessary
}

void gpio_pwmconfig(){
    P1->DIR |= BIT6;            //P1.6 pins output the rest are input
    P1->REN |= BIT6;                   // Enable internal pull-up/down resistors
    P1->OUT |= BIT6;                   //Select pull-up mode for P1.6
    P1->IE |= BIT6;                       // P1.6 interrupt enabled
    P1->IES |= BIT6;                     // P1.6 Hi/lo edge
    P1->IFG &= ~BIT6;                  // P1.6 IFG cleared

    P1->DIR |= BIT5;
    P1->REN |= BIT5;
    P1->OUT |= BIT5;
    P1->IE |= BIT5;
    P1->IES |= BIT5;
    P1->IFG &= ~BIT5;
}
/*
void gpio_LEDconfig() {
    P2->SEL0&=~BIT6&~BIT4;
    P2->SEL1&=~BIT6&~BIT4;
    P2->DIR|=BIT6|BIT4;
    P2->OUT&=~BIT6&~BIT4;

    P5->SEL0&=~BIT6;
    P5->SEL1&=~BIT6;
    P5->DIR|=BIT6;
    P5->OUT&=~BIT6;
}
void gpio_LEDout(uint8_t R, uint8_t G, uint8_t B) {
    if(R) P2->OUT|=BIT6;
    else P2->OUT&=~BIT6;

    if(G) P2->OUT|=BIT4;
    else P2->OUT&=~BIT4;

    if(B) P5->OUT|=BIT6;
    else P5->OUT&=~BIT6;
}

void gpio_registerInterrupt(uint8_t portNumber, uint8_t pin, intHandler function) {
    interruptHandlers[portNumber][pin]=function;
}
*/
void joystickcall(void){
    uint16_t value = joysticklocation(0,4);
    if(value == 3){
        //turn both motors a certain pwm
    }
    if(value == 2){
        //turn both motors a certain pwm
    }
    if(value == 1){
        //turn both motors a certain pwm
    }
    if(value == 0){
        //DO NOTHING
    }
}

void PORT1_IRQHandler() {
    int i;
    for(i=0;i<8;i++) //if ANY port is called
    {
        if(P1->IFG&(1<<i)) {
            joystickcall(); //above
            P1->IFG&=~(1<<i); //clear interrupt flag
        }
    }
}
