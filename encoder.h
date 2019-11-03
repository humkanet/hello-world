#ifndef _ENCODER_H
#define _ENCODER_H

/*
	ЭНКОДЕР

	Энкодер передает сигнал кодом Грея. Оцифровка этого кода идет с частотой
	дискретизации заданной ENCODER_SAMPLE_RATE. В качестве таймера используется
	Timer4
*/


#include <stdint.h>
#include "hal.h"


#define ENCODER_SAMPLE_RATE  1500
#define MAX_ENCODERS         4


typedef struct _ENCODER ENCODER;
typedef void   (*ENCODER_CALLBACK)(ENCODER *enc, int8_t cnt);


struct _ENCODER {
	HAL_PIN  pina;
	HAL_PIN  pinb;
	ENCODER_CALLBACK  callback;
};


inline void  encoder_isr(void);
inline void  encoder_init(void);
void         encoder_append(ENCODER *enc);
void         encoder_remove(ENCODER *enc);
void         encoder_reset(ENCODER *enc);
void         encoder_tick(void);

#endif
