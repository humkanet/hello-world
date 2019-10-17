#ifndef _BUTTON_H
#define _BUTTON_H

#include <stdint.h>


#define BUTTON_DEBOUNCE_MS  50


typedef enum {
	BUTTON_PORTA = 0x00,
	BUTTON_PORTB = 0x01,
	BUTTON_PORTC = 0x02
} BUTTON_PORT;


typedef enum {
	BUTTON_EVENT_UP   = 0x01,
	BUTTON_EVENT_DOWN = 0x02
} BUTTON_EVENT;


typedef enum {
	BUTTON_STAGE_IDLE     = 0x00,
	BUTTON_STAGE_DEBOUNCE = 0x01,
	BUTTON_STAGE_DOWN     = 0x02,
	BUTTON_STAGE_RESET    = 0x03
} BUTTON_STAGE;


typedef struct _BUTTON BUTTON;
typedef void   (*BUTTON_CALLBACK)(BUTTON *btn, BUTTON_EVENT e);


struct _BUTTON {
	uint8_t          port : 2;
	uint8_t          pin  : 3;
	uint8_t          stage: 2;
	uint16_t         msec;
	BUTTON_CALLBACK  event;
};


void         button_init(BUTTON *btn);
void         button_tick(BUTTON *btn, uint16_t msec);
uint8_t      button_pin(BUTTON *btn);
inline void  button_reset(BUTTON *btn);

#endif /* _BUTTON_H */
