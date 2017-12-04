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
    twentyfive = 50,
    fifty = 100,
    max = 190
};

void timerA0_config();
void timerA1_config();
void pwm(uint8_t inputvalue);
uint16_t retriever(void);

#endif /* TIMER_H_ */
