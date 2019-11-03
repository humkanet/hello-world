#ifndef _BUTTON_H
#define _BUTTON_H

#include <stdint.h>
#include "hal.h"


#define BUTTON_DEBOUNCE_MS  50
#define MAX_BUTTONS         4


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
	HAL_PIN       pin;
	BUTTON_STAGE  stage;
	uint16_t      msec;
	BUTTON_CALLBACK  callback;
};


inline void  button_init(void);
inline void  button_tick(uint16_t msec);
void         button_append(BUTTON *btn);
void         button_remove(BUTTON *btn);
void         button_reset(BUTTON *btn);

#endif /* _BUTTON_H */
