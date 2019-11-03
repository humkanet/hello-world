#include <xc.h>
#include "button.h"


#define U16(x)  ((uint16_t) (x))


typedef struct {
	BUTTON   *button;
} ENTRY;


volatile struct {
	ENTRY  entries[MAX_BUTTONS];
} buttons;


inline void button_init()
{
	volatile ENTRY *entry = buttons.entries;
	for (uint8_t n=0; n<MAX_BUTTONS; n++, entry++){
		entry->button = NULL;	
	}
}


inline void button_tick(uint16_t msec)
{
	volatile ENTRY *entry = buttons.entries;
	for (uint8_t n=0; n<MAX_BUTTONS; n++, entry++){
		BUTTON  *btn = entry->button;
		if (btn==NULL) continue;
		// Считываем значение пина
		uint8_t pin = pin_read(btn->pin);
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
					if (btn->callback) btn->callback(btn, BUTTON_EVENT_DOWN);
					btn->stage = BUTTON_STAGE_DOWN;
				}
				else btn->stage = BUTTON_STAGE_IDLE;
				break;
			/* Кнопка нажата */
			case BUTTON_STAGE_DOWN:
				// Ждем когда на пине появится высокий уровень и переводим
				// кнопку в режим простоя
				if (pin){
					if (btn->callback) btn->callback(btn, BUTTON_EVENT_UP);
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
}


void button_append(BUTTON *btn)
{
	volatile ENTRY *entry = buttons.entries;
	for (uint8_t n=0; n<MAX_BUTTONS; n++, entry++){
		// Ищем свободный слот
		if (entry->button==NULL){
			// Настраиваем пины
			pin_config(btn->pin, HAL_PIN_TRISTATE | HAL_PIN_PULLUP);
			// Инициализируем кнопку
			btn->stage = BUTTON_STAGE_IDLE;
			// Добавляем кнопку в обработку
			entry->button = btn;
			break;
		}
	}
}


void button_remove(BUTTON *btn)
{
	volatile ENTRY *entry = buttons.entries;
	for (uint8_t n=0; n<MAX_BUTTONS; n++, entry++){
		// Ищем кнопку
		if (entry->button==btn){
			entry->button = NULL;
			break;
		}
	}
}


void button_reset(BUTTON *btn)
{
	btn->stage = BUTTON_STAGE_IDLE;
}
