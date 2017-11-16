/*
 * port.c
 *
 *  Created on: Nov 15, 2017
 *      Author: Brandon Lewien
 */

#include "msp.h"

void GPIO_Configure()
{
    P1->SEL0 |= BIT6;

}
