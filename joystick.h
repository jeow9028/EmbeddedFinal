#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "msp.h"
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

extern volatile uint16_t _nadc[32];

void joystick_configure(void);
int8_t joysticklocation(uint8_t channelx, uint8_t channely);
void pwm(uint8_t inputvalue);

#endif /* JOYSTICK_H_ */
