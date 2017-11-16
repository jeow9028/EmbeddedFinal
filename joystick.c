/*
 * joystick.c
 *
 *  Created on: 11/15/17
 *      Author: Brandon Lewien
 */

#include "adc.h"
#include "msp.h"
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <adc.h>

/*
 * Bitmapping is in overview for ADC14
 */
void GPIO_configure(void){
    P4->SEL0 |= BIT4;
    P4->SEL1 |= BIT4;
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;

    P4->SEL0 |= BIT6;
    P4->SEL1 |= BIT6;
}

/*
 * Default position reads for x=8314 and for y=8147
 * If joystick returns 1 location is Up-Right
 * If joystick returns 2 location is Up-Left
 * If joystick returns 3 location is Down-Left
 * If joystick returns 4 location is Down-Right
 * If joystick returns 0 location is Neutral
 * Channelx takes an input from main that compares values for the left and right movement
 * Channely takes an input from main that compares values for the up and down movement
 */
uint8_t joysticklocation(uint8_t channelx, uint8_t channely){

    uint8_t location = 0;
    if((ADC_getN(channelx) > 9000) && (ADC_getN(channely) > 9000)){
        location = 1;
        return location;
    }
    else if((ADC_getN(channelx) < 8000) && (ADC_getN(channely) > 9000)){
        location = 2;
        return location;
    }
    else if((ADC_getN(channelx) < 8000) && (ADC_getN(channely) < 8000)){
        location = 3;
        return location;
    }
    else if((ADC_getN(channelx) > 9000) && (ADC_getN(channely) < 8000)){
        location = 4;
        return location;
    }
    else{
        return 0;
    }
}
uint8_t light(uint8_t channellight){
    uint8_t storer = 0;
    storer = ADC_getN(channellight);
    return storer;

}
