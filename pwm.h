#ifndef _PWM_H
#define	_PWM_H

#include <stdint.h>


inline void  pwm_init(void);
void         pwm_start(void);
void         pwm_stop(void);
void         pwm_set(uint16_t ton, uint16_t toff);


#endif	/* _PWM_H */
