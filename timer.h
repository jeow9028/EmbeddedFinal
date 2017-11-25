/*
 * lcdconfig.h
 *
 *  Created on: Nov 3, 2017
 *      Author: Brandon
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "gpio1.h"

enum dutycycle{
    zero = 0,
    fifty = 213,
    ninetynine = 450
};

void timerA0_config();
void timerA1_config();
void pwm(uint8_t inputvalue);

#endif /* TIMER_H_ */
