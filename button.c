#include <xc.h>
#include "button.h"


#define U16(x)  ((uint16_t) (x))


void button_init(BUTTON *btn)
{	
	uint8_t pin = (1<<btn->pin);
	switch(btn->port){
		#ifdef TRISA
		case BUTTON_PORTA:
			TRISA   |= pin;
			WPUA    |= pin;
			ANSELA  &= ~pin;
			SLRCONA |= pin;
			break;
		#endif
		#ifdef TRISB
		case BUTTON_PORTB:
			TRISB   |= pin;
			WPUB    |= pin;
			ANSELB  &= ~pin;
			SLRCONB |= pin;
			break;
		#endif
		#ifdef TRISC
		case BUTTON_PORTC:
			TRISC   |= pin;
			WPUC    |= pin;
			ANSELC  &= ~pin;
			SLRCONC |= pin;
			break;
		#endif
	}
	btn->stage = BUTTON_STAGE_IDLE;
}


void button_tick(BUTTON *btn, uint16_t msec)
{
	// Считываем значение пина
	uint8_t pin = button_pin(btn);
	// Конечный автомат для кнопки
	switch(btn->stage){
		/* Кнопка простивает */ 
		case BUTTON_STAGE_IDLE:
			// Если на пине низкий уровень, то переводим в режим
			// подавления дребезга
			if (!pin){
				btn->msec  = msec;
				btn->stage = BUTTON_STAGE_DEBOUNCE;
			}
			break;
		/* Режим подавления дребезга */
		case BUTTON_STAGE_DEBOUNCE:
			// Ждем интервал подавления дребезга
			if (U16(msec-btn->msec)<BUTTON_DEBOUNCE_MS) return;
			// Если после указанного интервала на пине низкий уровень,
			// то помечаем кнопку как нажатую, иначе переводим в
			// режим простоя
			if (!pin){
				if (btn->event) btn->event(btn, BUTTON_EVENT_DOWN);
				btn->stage = BUTTON_STAGE_DOWN;
			}
			else btn->stage = BUTTON_STAGE_IDLE;
			break;
		/* Кнопка нажата */
		case BUTTON_STAGE_DOWN:
			// Ждем когда на пине появится высокий уровень и переводим
			// кнопку в режим простоя
			if (pin){
				if (btn->event) btn->event(btn, BUTTON_EVENT_UP);
				btn->stage = BUTTON_STAGE_IDLE;
			}
			break;
		/* Режим сброса кнопки */
		case BUTTON_STAGE_RESET:
			// Ждем когда на пине появится высокий уровень и переводим
			// кнопку в режим простоя
			if (pin) btn->stage = BUTTON_STAGE_IDLE;
			break;
	}
}


uint8_t button_pin(BUTTON *btn)
{
	uint8_t  mask = 1<<btn->pin;
	switch(btn->port){
		#ifdef TRISA
		case BUTTON_PORTA:
			return PORTA & mask;
		#endif
		#ifdef TRISB
		case BUTTON_PORTB:
			return PORTB & mask;
		#endif
		#ifdef TRISC
		case BUTTON_PORTC:
			return PORTC & mask;
		#endif
	}
	// Пин не найден, возвращаем высокий уровень
	return 0xFF;
}


inline void button_reset(BUTTON *btn)
{
	btn->stage = BUTTON_STAGE_IDLE;
}
