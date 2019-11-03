#include <xc.h>
#include "encoder.h"


#define U8(x)    ((uint8_t) (x))

/*
	Расчет количества тактов таймера для достижения
	необходимой частоты дискретизации.
	МК работает на частоте _XTAL_FREQ, тактируется на 
	1/4 частоты МК, таким образом количество тактов
	TIMER_TICKS = (_XTAL_FREQ/(ENCODER_SAMPLE_RATE*4))
*/
#define TIMER_TICKS  (_XTAL_FREQ/(4UL*ENCODER_SAMPLE_RATE))
#if TIMER_TICKS<=256
	#define TIMER_DIV   1
	#define TIMER_CKPS  0x00
#elif TIMER_TICKS/4<=256
	#define TIMER_DIV   4
	#define TIMER_CKPS  0x01
#elif TIMER_TICKS/16<=256
	#define TIMER_DIV   16
	#define TIMER_CKPS  0x02
#elif TIMER_TICKS/64<=256
	#define TIMER_DIV   64
	#define TIMER_CKPS  0x03
#else
	#error "ENCODER_SAMPLE_RATE too low!"
#endif


typedef struct {
	ENCODER  *encoder;
	int8_t   cnt;
	int8_t   step;
	union {
		struct {
			uint8_t  state:2;
			uint8_t  lock :1;
		};
		uint8_t  flags;
	};
} ENTRY;


volatile struct {
	ENTRY  entries[MAX_ENCODERS];
} encoder;


const int8_t ENCODER_STEP[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };


inline void encoder_isr()
{
	if (TMR4IF){
		// Сбрасываем флаг прерывания
		TMR4IF = 0;
		// Обрабатываем энкодеры
		volatile ENTRY *entry = encoder.entries;
		for(uint8_t n=0; n<MAX_ENCODERS; n++, entry++){
			// Обрабатываем энкодер
			ENCODER  *encoder = entry->encoder;
			if (encoder==NULL) continue;
			// Считываем пины
			uint8_t  state = (pin_read(encoder->pina)<<1) | pin_read(encoder->pinb);
			// Пины не изменились, пропускаем
			if (state==entry->state) continue;
			// Счетчик шагов
			entry->step += ENCODER_STEP[(entry->state<<2) | state];
			if (entry->step>3){
				entry->step = 0;
				if (entry->cnt<100) entry->cnt ++;
			}
			else if (entry->step<-3){
				entry->step = 0;
				if (entry->cnt>-100) entry->cnt --;	
			}
			// Сохраняем значеие пинов
			entry->state = state;
		}
	}
}


inline void encoder_init()
{
	// Подаем питание на периферию
	#ifdef TMR4MD
	TMR4MD = 0;
	asm("nop");
	#endif
	// Инициализация энкодеров
	volatile ENTRY *entry = encoder.entries;
	for (uint8_t n=0; n<MAX_ENCODERS; n++, entry++){
		entry->encoder = NULL;
	}
	// Настраиваем таймер
	T4CON  = 0x00 | (TIMER_CKPS & 0x03);
	PR4    = U8(TIMER_TICKS/TIMER_DIV)-1;
	TMR4   = 0;
	TMR4IF = 0;
	TMR4IE = 1;
}


void encoder_append(ENCODER *enc)
{
	volatile ENTRY *entry = encoder.entries;
	for (uint8_t n=0; n<MAX_ENCODERS; n++, entry++){
		// Ищем свободный слот
		if (entry->encoder==NULL){
			// Настраиваем пины
			pin_config(enc->pina, HAL_PIN_TRISTATE | HAL_PIN_PULLUP);
			pin_config(enc->pinb, HAL_PIN_TRISTATE | HAL_PIN_PULLUP);
			// Инициализируем энкодер
			entry->cnt   = 0;
			entry->step  = 0;
			entry->flags = 0x00;
			// Сохраняем энкодер
			entry->encoder = enc;
			break;
		}
	}
}


void encoder_remove(ENCODER *enc)
{
	volatile ENTRY *entry = encoder.entries;
	for (uint8_t n=0; n<MAX_ENCODERS; n++, entry++){
		// Ищем энкодер
		if (entry->encoder==enc){
			// Убираем энкодер из списка обработки
			entry->encoder = NULL;
			break;
		}
	}
}


void encoder_reset(ENCODER *enc)
{
	volatile ENTRY *entry = encoder.entries;
	for (uint8_t n=0; n<MAX_ENCODERS; n++, entry++){
		// Ищем энкодер
		if (entry->encoder==enc){
			entry->lock = 0x01;
			entry->cnt  = 0;
			entry->step = 0;
			entry->lock = 0x00;
		}
	}
}


void encoder_tick()
{
	volatile ENTRY *entry = encoder.entries;
	for (uint8_t n=0; n<MAX_ENCODERS; n++, entry++){
		ENCODER *enc = entry->encoder;
		if (enc!=NULL){
			// Считываем значение
			entry->lock = 0x01;
			int8_t  cnt = entry->cnt;
			entry->cnt  = 0;
			entry->lock = 0x00;
			// Вызываем обработку
			if (cnt && enc->callback) enc->callback(enc, cnt);
		}
	}
}
