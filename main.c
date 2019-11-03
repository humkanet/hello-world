#include <xc.h>
#include "config.h"
#include "pwm.h"
#include "clock.h"
#include "button.h"
#include "encoder.h"


void __interrupt() isr()
{
	clock_isr();
	encoder_isr();
}


inline void setup()
{
	// Инициализация периферии
	pwm_init();
	clock_init();
	encoder_init();
	button_init();
	// Режим сна - IDLE
	IDLEN = 1;
	// Разрешаем прерывания от периферии
	PEIE  = 1;
}


void button_callback(BUTTON *btn, BUTTON_EVENT e)
{
	// Если кнопка зажата вниз - запускаем ШИМ,
	// иначе останавливаем
	if (e==BUTTON_EVENT_DOWN) pwm_start();
	else pwm_stop();
}

void encoder_callback(ENCODER *enc, int8_t cnt)
{
	
}


void main()
{
	di();
	setup();
	ei();

	// Кнопка, пин RC2
	BUTTON btn = {
		.pin      = { .port=HAL_PORTC, .pin=2 },
		.callback = button_callback
	};
	button_append(&btn);

	// Энкодер, пин A = RC0, пин B = RC1
	ENCODER enc = {
		.pina     = { .port=HAL_PORTC, .pin=0 },
		.pinb     = { .port=HAL_PORTC, .pin=1 },
		.callback = encoder_callback
	};
	encoder_append(&enc);

	// ШИМ: Ton=20мсек, Toff=10мсек
	pwm_set(20,10);

	for(;;){
		// Считываем системные часы
		uint16_t msec = clock_msec();
		// Обрабатываем кнопки
		button_tick(msec);
		// Обрабатываем энкодеры
		encoder_tick();
		// Спим до след. события
		SLEEP();
	}
}
