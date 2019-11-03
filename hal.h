#ifndef _HAL_H
#define _HAL_H

#include <stdint.h>


typedef enum {
	HAL_PORTA,
	HAL_PORTB,
	HAL_PORTC
} HAL_PORT;


typedef struct {
	uint8_t  port:2;
	uint8_t  pin :6;
} HAL_PIN;


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
