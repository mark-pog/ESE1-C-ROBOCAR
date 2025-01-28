/*
 * basicFunctions.h
 *
 * Created: 23.01.2025 14:17:33
 *  Author: marcu
 */ 


#ifndef BASICFUNCTIONS_H_
#define BASICFUNCTIONS_H_


extern void digitalWrite(uint8_t pin, uint8_t value);

extern void pinMode(uint8_t pin, uint8_t value);

extern int digitalRead(int pin);

extern unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);

extern void millis_init(void);

extern inline uint32_t millis(void);

extern void pwm_init(void);

#endif /* BASICFUNCTIONS_H_ */