#ifndef _HAL_H
#define _HAL_H

#include <stdint.h>

#define HAL_MK_PIN(port, pin)  ((HAL_PIN) ((port&0x03)|(pin<<2)))
#define __HAL_PORT(x)          ((uint8_t) x&0x03)
#define __HAL_PIN(x)           ((uint8_t) x>>2)


typedef uint8_t HAL_PIN;


typedef enum {
	HAL_PORTA,
	HAL_PORTB,
	HAL_PORTC
} HAL_PORT;


typedef enum {
	HAL_PIN_LATCH     = 0x01,
	HAL_PIN_PULLUP    = 0x02,
	HAL_PIN_OPENDRAIN = 0x04,
	HAL_PIN_TRISTATE  = 0x08,
	HAL_PIN_ANALOG    = 0x10,
	HAL_PIN_SLEWRATE  = 0x20
} HAL_PIN_CONFIG;


void     pin_config(HAL_PIN pin, uint8_t cfg);
void     pin_set(HAL_PIN pin, uint8_t cfg, uint8_t val);
void     pin_write(HAL_PIN pin, uint8_t val);
uint8_t  pin_read(HAL_PIN pin);

#endif
